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

    private function getServiceName()
    {
        return "folder2ram";
    }

    private function getXPath()
    {
        return sprintf( "//services/%s", $this->getPluginName() );
    }

    private function getEventMessagePath()
    {
        return sprintf( "org.openmediavault.services.%s", $this->getPluginName() );
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
		$cmd = sprintf("folder2ram -refersh && folder2ram -mountall", $this->getName());
		
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

        $initd = new OMVSysVInitScript($this->getServiceName(), true);
        $initd->exec();
    }

    public function stopService()
    {
    
        global $xmlConfig;
		
// these commands will shutdown folder2ram
		
		$cmd = sprintf("folder2ram -clean", $this->getName());
		
		if (0 !== $this->exec($cmd, $output)) {
			throw new OMVException(
				OMVErrorMsg::E_EXEC_FAILED,
				$cmd,
				implode("\n", $output)
			);
		}
    
        $initd = new OMVSysVInitScript($this->getServiceName());
        $initd->exec();
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
