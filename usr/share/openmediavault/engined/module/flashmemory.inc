<?php

/**
 * Copyright (C) 2015 OpenMediaVault Plugin Developers
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

require_once "openmediavault/config.inc";
require_once "openmediavault/error.inc";
require_once "openmediavault/initscript.inc";
require_once "openmediavault/module.inc";
require_once "openmediavault/util.inc";

class OMVModuleFlashmemory extends OMVModuleServiceAbstract implements
    OMVINotifyListener
{
    public function getName()
    {
        return "flashmemory";
    }

    private function getPluginName()
    {
	return strtolower( $this->getName() );
    }

    private function getXPath()
    {
        return sprintf( "//services/%s", $this->getName() );
    }

    private function getEventMessagePath()
    {
        return sprintf( "org.openmediavault.services.%s", $this->getName() );
    }

    public function applyConfig()
    {
        $cmd = sprintf("omv-mkconf %s 2>&1", $this->getName());

        if (0 !== $this->exec($cmd, $output)) {
            throw new OMVException(
                OMVErrorMsg::E_EXEC_FAILED,
                $cmd,
                implode("\n", $output)
            );
        }
    }

    public function startService()
    {
        global $xmlConfig;
        
// when (if) OMV runs these commands, this will power up folder2ram

	$cmd = sprintf(folder2ram -refresh 2>&1 && folder2ram -mountall 2>&1");

        if (0 !== $this->exec($cmd, $output)) {
            throw new OMVException(
                OMVErrorMsg::E_EXEC_FAILED,
                $cmd,
                implode("\n", $output)
            );
        }


        $object = $xmlConfig->get($this->getXPath());

        if (is_null($object)) {
            throw new OMVException(
                OMVErrorMsg::E_CONFIG_GET_OBJECT_FAILED,
                $this->getXPath()
            );
        }

        if (!boolval($object["enable"])) {
            return;
        }


    }

    public function stopService()
    {

// when (if) OMV runs these commands, it will power down folder2ram.

	$cmd = sprintf("folder2ram -clean 2>&1");

        if (0 !== $this->exec($cmd, $output)) {
            throw new OMVException(
                OMVErrorMsg::E_EXEC_FAILED,
                $cmd,
                implode("\n", $output)
            );
        }

    }

    public function bindListeners(OMVNotifyDispatcher $dispatcher)
    {
        $moduleMgr = &OMVModuleMgr::getInstance();

        $dispatcher->addListener(
            OMV_NOTIFY_MODIFY,
            $this->getEventMessagePath(),
            array($this, "setDirty")
        );
    }
}

// Register module
$moduleMgr = &OMVModuleMgr::getInstance();
$moduleMgr->registerModule(new OMVModuleFlashmemory());

?>
