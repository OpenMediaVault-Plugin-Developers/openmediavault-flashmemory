# @license   http://www.gnu.org/licenses/gpl.html GPL Version 3
# @author    OpenMediaVault Plugin Developers <plugins@omv-extras.org>
# @copyright Copyright (c) 2019-2022 OpenMediaVault Plugin Developers
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

configure_rrd_dir:
  file.directory:
    - name: "/var/lib/openmediavault/rrd"
    - makedirs: True

configure_samba_dir:
  file.directory:
    - name: "/var/cache/samba"
    - makedirs: True

configure_folder2ram_dir:
  file.directory:
    - name: "/etc/folder2ram"
    - makedirs: True

configure_flashmemory:
  file.managed:
    - name: "/etc/folder2ram/folder2ram.conf"
    - contents: |
        {{ pillar['headers']['auto_generated'] }}
        {{ pillar['headers']['warning'] }}
        #<type>		<mount point>		<options>
        tmpfs		/var/log
        tmpfs		/var/tmp
        tmpfs		/var/lib/openmediavault/rrd
        tmpfs		/var/spool
        tmpfs		/var/lib/rrdcached/
        tmpfs		/var/lib/monit
        tmpfs		/var/cache/samba
    - user: root
    - group: root
    - mode: 644

folder2ram_enable_systemd:
  cmd.run:
    - name: "/sbin/folder2ram -enablesystemd"

folder2ram_mountall:
  cmd.run:
    - name: "/sbin/folder2ram -mountall"

systemd-reload:
  cmd.run:
    - name: systemctl daemon-reload
