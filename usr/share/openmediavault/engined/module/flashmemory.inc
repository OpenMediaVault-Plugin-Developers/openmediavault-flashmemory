<?php

/**
 * Copyright (C) 2013-2022 OpenMediaVault Plugin Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

class OMVModuleFlashMemory extends \OMV\Engine\Module\ServiceAbstract
    implements \OMV\Engine\Module\IServiceStatus
{
    public function getName()
    {
        return "flashmemory";
    }

    public function getStatus()
    {
        $cmd = new \OMV\System\Process("grep -c folder2ram /proc/mounts");
        $cmd->execute($output, $exitStatus);
        $systemCtl = new \OMV\System\SystemCtl("folder2ram_startup");
        return array(
            "name" => $this->getName(),
            "title" => gettext("FlashMemory"),
            "enabled" => true,
            "running" => intval($output[0]) > 0 ? true : false
        );
    }
}
