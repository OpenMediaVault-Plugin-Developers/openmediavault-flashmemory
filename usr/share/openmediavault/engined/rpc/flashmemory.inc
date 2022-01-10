<?php
/**
 * Copyright (C) 2015-2022 OpenMediaVault Plugin Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
class OMVRpcServiceFlashmemory extends \OMV\Rpc\ServiceAbstract
{
    public function getName()
    {
        return "Flashmemory";
    }

    public function initialize()
    {
        $this->registerMethod("get");
    }

    public function get($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        $cmd = new \OMV\System\Process("grep -c folder2ram /proc/mounts");
        $cmd->execute($output, $exitStatus);
        return [
            "status" => intval($output[0]) > 1 ? gettext('running') : gettext('not running')
        ];
    }
}
