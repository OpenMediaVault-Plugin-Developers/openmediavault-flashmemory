<?php
/**
 * Copyright (C) 2015-2025 openmediavault plugin developers
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
        $this->registerMethod("getDirList");
        $this->registerMethod("setDir");
        $this->registerMethod("getDir");
        $this->registerMethod("deleteDir");

        $this->registerMethod("get");
        $this->registerMethod("syncall");
    }

    public function getDirList($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, "rpc.common.getlist");
        // Get the configuration object.
        $db = \OMV\Config\Database::getInstance();
        $objects = $db->getAssoc("conf.service.flashmemory.dir");
        // Filter the result.
        return $this->applyFilter($objects,$params['start'],$params['limit'],
            $params['sortfield'],$params['sortdir']);
    }

    public function getDir($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, "rpc.common.objectuuid");
        // Get the configuration object.
        $db = \OMV\Config\Database::getInstance();
        return $db->getAssoc("conf.service.flashmemory.dir", $params['uuid']);
    }

    public function setDir($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, "rpc.flashmemory.setdir");
        if (strpos($params['path'], '/opt/zram') === 0) {
            throw new \OMV\Exception(gettext("Directory cannot start with /opt/zram!"));
        }
        if ($params['ztype'] == 'swap') {
            $params['path'] = '';
        }
        // Prepare the configuration object.
        $object = new \OMV\Config\ConfigObject("conf.service.flashmemory.dir");
        $object->setAssoc($params);
        // Set the configuration object
        $db = \OMV\Config\Database::getInstance();
        if (TRUE === $object->isNew()) {
            // Check uniqueness:
            $db->assertIsUnique($object, "uuid");
            $db->assertIsUnique($object, "path");
        }
        $db->set($object);
        // Return the configuration object.
        return $object->getAssoc();
    }

    public function deleteDir($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, "rpc.common.objectuuid");
        // Delete the configuration object.
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get("conf.service.flashmemory.dir", $params['uuid']);
        $db->delete($object);
        // Return the deleted configuration object.
        return $object->getAssoc();
    }

    public function get($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        $systemCtl = new \OMV\System\SystemCtl("zram-config");
        return [
            "status" => $systemCtl->isActive() ? gettext('running') : gettext('not running')
        ];
    }

    public function syncall($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);
        return $this->execBgProc(function($bgStatusFilename, $bgOutputFilename)
            use ($params) {
                $cmd = new \OMV\System\Process("/usr/sbin/zram-config sync");
                $cmd->setQuiet(true);
                $cmdLine = $cmd->getCommandLine();
                if (0 !== $this->exec($cmdLine, $output, $bgOutputFilename)) {
                    throw new \OMV\ExecException($cmdLine, $output);
                }
                return $output;
            }
        );
    }
}
