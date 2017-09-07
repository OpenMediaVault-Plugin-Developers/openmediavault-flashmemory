/**
 * Copyright (C) 2015-2017 OpenMediaVault Plugin Developers
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

// require("js/omv/WorkspaceManager.js")
// require("js/omv/workspace/form/Panel.js")
// require("js/omv/data/Store.js")
// require("js/omv/data/Model.js")

Ext.define("OMV.module.admin.storage.flashmemory.Settings", {
    extend : "OMV.workspace.form.Panel",
    uses   : [
        "OMV.data.Model",
        "OMV.data.Store"
    ],

    rpcService   : "Flashmemory",
    rpcGetMethod : "getSettings",

    hideOkButton : true,

    getFormItems : function() {
        return [{
            xtype    : "fieldset",
            title    : "Details",
            defaults : {
                labelSeparator : ""
            },
            items : [{
                xtype       : "textfield",
                name        : "root",
                fieldLabel  : _("Root"),
                submitValue : false,
                readOnly    : true
            },{
                xtype       : "textfield",
                name        : "written",
                fieldLabel  : _("Write Kbytes"),
                submitValue : false,
                readOnly    : true
            }]
        },{
            xtype         : "fieldset",
            title         : _("Notes (optional)"),
            fieldDefaults : {
                labelSeparator : ""
            },
            items         : [{
                border : false,
                html   : "<p>" + _("Fstab (/etc/fstab) needs to be changed manually. Following these steps to change:") +
                         "<ol>" +
                           "<li>" + _("Login as root locally or via ssh") + "</li>" +
                           "<li>" + _("Execute the following command:  <b>nano /etc/fstab</b>") + "</li>" +
                           "<li>" + _("Add noatime and nodiratime to root options.  See before and after example lines:") + "</li>" +
                             "<dl>" +
                               "<dt>" + _("BEFORE:") + "</dt>" +
                               "<dd>" + "UUID=ccd327d4-a1ed-4fd2-b356-3b492c6f6c34  /  ext4  errors=remount-ro  0  1" + "</dd>" +
                               "<dt>" + _("AFTER:") + "</dt>" +
                               "<dd>" + "UUID=ccd327d4-a1ed-4fd2-b356-3b492c6f6c34  /  ext4  noatime,nodiratime,errors=remount-ro  0  1" + "</dd>" +
                             "</dl>" +
                           "<li>" + _("Comment out the swap partition.  See before and after example lines (only need to add a # to beginning of the line):") + "</li>" +
                             "<dl>" +
                               "<dt>" + _("BEFORE:") + "</dt>" +
                               "<dd>" + "UUID=a3c989d8-e12b-41d3-b021-098155d6b21b  none  swap  sw  0  0" + "</dd>" +
                               "<dt>" + _("AFTER:") + "</dt>" +
                               "<dd>" + "#UUID=a3c989d8-e12b-41d3-b021-098155d6b21b  none  swap  sw  0  0" + "</dd>" +
                             "</dl>" +
                          "<li>" + _("Ctrl-o to save") + "</li>" +
                          "<li>" + _("Ctrl-x to exit") + "</li>" +
                          "<li>" + _("Enable the plugin") + "</li>" +
                          "<li>" + _("Reboot") + "</li>" +
                         "</ol>" +
                         "</p>"
            }]
        }];
    }
});

OMV.WorkspaceManager.registerPanel({
    id        : "settings",
    path      : "/storage/flashmemory",
    text      : _("Settings"),
    position  : 10,
    className : "OMV.module.admin.storage.flashmemory.Settings"
});
