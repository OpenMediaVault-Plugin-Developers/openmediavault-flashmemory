#!/bin/sh

set -e

. /usr/share/openmediavault/scripts/helper-functions

SERVICE_NAME="flashmemory"
XPATH="/config/services/${SERVICE_NAME}"

if ! omv_config_exists "${XPATH}"; then
  omv_config_add_node "/config/services" "${SERVICE_NAME}"
  omv_config_add_node "${XPATH}" "dirs"

  object="<uuid>ce7325d6-d032-11ef-9402-fb9e0253d055</uuid>"
  object="${object}<enable>1</enable>"
  object="${object}<ztype>swap</ztype>"
  object="${object}<path></path>"
  object="${object}<memlimit>250</memlimit>"
  object="${object}<disksize>750</disksize>"
  omv_config_add_node_data "${XPATH}/dirs" "dir" "${object}"

  object="<uuid>d1d72880-d032-11ef-b216-dfdc0cb5988a</uuid>"
  object="${object}<enable>1</enable>"
  object="${object}<ztype>dir</ztype>"
  object="${object}<path>/var/tmp</path>"
  object="${object}<memlimit>250</memlimit>"
  object="${object}<disksize>750</disksize>"
  omv_config_add_node_data "${XPATH}/dirs" "dir" "${object}"

  object="<uuid>d2363d02-d032-11ef-8ab1-f7ed0db98337</uuid>"
  object="${object}<enable>1</enable>"
  object="${object}<ztype>dir</ztype>"
  object="${object}<path>/var/lib/openmediavault/rrd</path>"
  object="${object}<memlimit>50</memlimit>"
  object="${object}<disksize>150</disksize>"
  omv_config_add_node_data "${XPATH}/dirs" "dir" "${object}"

  object="<uuid>d287f0de-d032-11ef-b584-370df3ff4bfa</uuid>"
  object="${object}<enable>1</enable>"
  object="${object}<ztype>dir</ztype>"
  object="${object}<path>/var/lib/rrdcached/</path>"
  object="${object}<memlimit>50</memlimit>"
  object="${object}<disksize>150</disksize>"
  omv_config_add_node_data "${XPATH}/dirs" "dir" "${object}"

  object="<uuid>d2d9a01e-d032-11ef-908a-a3e0867c35a7</uuid>"
  object="${object}<enable>1</enable>"
  object="${object}<ztype>dir</ztype>"
  object="${object}<path>/var/lib/monit</path>"
  object="${object}<memlimit>50</memlimit>"
  object="${object}<disksize>150</disksize>"
  omv_config_add_node_data "${XPATH}/dirs" "dir" "${object}"

  object="<uuid>d32a95aa-d032-11ef-afe1-efb016e5b099</uuid>"
  object="${object}<enable>1</enable>"
  object="${object}<ztype>dir</ztype>"
  object="${object}<path>/var/cache/samba</path>"
  object="${object}<memlimit>50</memlimit>"
  object="${object}<disksize>150</disksize>"
  omv_config_add_node_data "${XPATH}/dirs" "dir" "${object}"

  object="<uuid>03d39020-d034-11ef-b5e5-8fdcc613b8b0</uuid>"
  object="${object}<enable>1</enable>"
  object="${object}<ztype>log</ztype>"
  object="${object}<path>/var/log</path>"
  object="${object}<memlimit>50</memlimit>"
  object="${object}<disksize>150</disksize>"
  omv_config_add_node_data "${XPATH}/dirs" "dir" "${object}"
fi

exit 0
