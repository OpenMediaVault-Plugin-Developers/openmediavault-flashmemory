# @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
# @author    Volker Theile <volker.theile@openmediavault.org>
# @copyright Copyright (c) 2009-2019 Volker Theile
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

configure_flashmemory:
  file.managed:
    - name: "/etc/folder2ram/folder2ram.conf"
    - source:
      - salt://{{ slspath }}/files/etc-folder2ram-folder2ram_conf.j2
    - template: jinja
    - context:
        config: {{ config | json }}
    - user: root
    - group: root
    - mode: 644

configure_rrd_dir:
  file.directory:
    - name: "/var/lib/openmediavault/rrd"

configure_netatalk_dir:
  file.directory:
    - name: "/var/lib/netatalk/CNID"

configure_samba_dir:
  file.directory:
    - name: "/var/cache/samba"

remove_cron_apt_file:
  file.absent:
    - name: "/etc/cron-apt/action.d/3-download"

clean_apt_cache:
  cmd.run:
    - name: /usr/bin/apt-get clean
