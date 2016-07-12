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
class OMVRpcServiceFlashmemory extends \OMV\Rpc\ServiceAbstract
{
    public function getName()
    {
        return "Flashmemory";
    }

    public function initialize()
    {
        $this->registerMethod("getSettings");
        $this->registerMethod("setSettings");
    }

    public function getSettings($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        // Get root drive and data written
        $cmd = new \OMV\System\Process("omv-showroot");
        $cmd->setQuiet(true);
        $cmd->execute($output);
        $root = explode("/", $output[0]);
        // Get session_write_kbytes for root drive
        $written = gettext("n/a");
        $writefile = sprintf("/sys/fs/ext4/%s/session_write_kbytes", $root[2]);
        if (file_exists($writefile)) {
            $written = file_get_contents($writefile);
        }
        // Get the configuration object.
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get("conf.service.flashmemory");
        $object->set("root", $output[0]);
        $object->set("written", $written);
        return $object->getAssoc();
    }

    public function setSettings($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, "rpc.flashmemory.setsettings");
        // Get the existing configuration object.
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get("conf.service.flashmemory");
        $object->setAssoc($params);
        $db->set($object);
        // Return the configuration object.
        return $object->getAssoc();
    }
}
