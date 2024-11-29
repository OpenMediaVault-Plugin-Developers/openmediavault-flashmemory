# @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
# @author    OpenMediaVault Plugin Developers <plugins@omv-extras.org>
# @copyright Copyright (c) 2019-2024 OpenMediaVault Plugin Developers
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

{% set extradirs = salt['pillar.get']('default:OMV_FM_EXTRA_DIRS', '') %}
{% set tabs = '\t\t' %}

configure_rrd_dir:
  file.directory:
    - name: "/var/lib/openmediavault/rrd"
    - makedirs: True

configure_samba_dir:
  file.directory:
    - name: "/var/cache/samba"
    - makedirs: True

configure_flashmemory:
  file.managed:
    - name: "/etc/ztab"
    - contents: |
        {{ pillar['headers']['auto_generated'] }}
        {{ pillar['headers']['warning'] }}
        # swap  alg      mem_limit  disk_size  swap_priority  page-cluster  swappiness
        swap    lzo-rle  250M		    750M       75             0             150
        # dir  alg      mem_limit  disk_size  target_dir                   bind_dir
        dir    lzo-rle  50M        150M       /var/tmp                     /opt/zram/vartmp.bind
        dir    lzo-rle  50M        150M       /var/lib/openmediavault/rrd  /opt/zram/rrd.bind
        dir    lzo-rle  50M        150M       /var/spool                   /opt/zram/spool.bind
        dir    lzo-rle  50M        150M       /var/lib/rrdcached/          /opt/zram/rrdcached.bind
        dir    lzo-rle  50M        150M       /var/lib/monit               /opt/zram/monit.bind
        dir    lzo-rle  50M        150M       /var/cache/samba             /opt/zram/samba.bind
        # log	 alg      mem_limit  disk_size  target_dir                   bind_dir                  oldlog_dir
        log    lzo-rle  50M        150M       /var/log                     /opt/zram/log.bind        /opt/zram/oldlog
        {%- set count = 0 %}
        {%- for path in extradirs.split(',') %}
        {%- if path.strip() | length > 1 %}
        {%- set count = count + 1 %}
        dir    lzo-rle  50M        150M       {{ path.strip() }}           /opt/zram/extra_{{ count }}.bind
        {%- endif %}
        {%- endfor %}
    - user: root
    - group: root
    - mode: 644

start_zram_config_service:
  service.running:
    - name: zram-config
    - enable: True
    - watch:
      - file: configure_flashmemory
