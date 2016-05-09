<?php

/**
 * Copyright (C) 2015-2016 OpenMediaVault Plugin Developers
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
class OMVModuleFlashmemory extends \OMV\Engine\Module\ServiceAbstract
    implements \OMV\Engine\Notify\IListener
{
    public function getName()
    {
        return "Flashmemory";
    }

    public function applyConfig()
    {
        $cmd = new \OMV\System\Process("omv-mkconf", "flashmemory");
        $cmd->setRedirect2to1(TRUE);
        $cmd->execute();
    }

    public function startService()
    {
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get("conf.service.flashmemory");
        if (TRUE !== $object->get("enable"))
            return;

        // start folder2ram
        $cmd = sprintf("folder2ram -enablesystemd 2>&1");
        if (0 !== $this->exec($cmd, $output)) {
            throw new \OMV\Exception(implode("\n", $output));
        }
    }

    public function stopService()
    {
        // stop folder2ram
        $cmd = sprintf("folder2ram -disablesystemd 2>&1");
        if (0 !== $this->exec($cmd, $output)) {
            throw new \OMV\Exception(implode("\n", $output));
        }
    }

    public function bindListeners(\OMV\Engine\Notify\Dispatcher $dispatcher)
    {
        $dispatcher->addListener(
            OMV_NOTIFY_MODIFY,
            "org.openmediavault.services.flashmemory",
            [ $this, "setDirty" ]
        );
    }
}
