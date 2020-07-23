/**
 * Copyright (C) 2015-2020 OpenMediaVault Plugin Developers
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
    extend: "OMV.workspace.form.Panel",
    uses: [
        "OMV.data.Model",
        "OMV.data.Store"
    ],

    rpcService: "Flashmemory",
    rpcGetMethod: "getSettings",

    hideOkButton: true,

    getFormItems: function() {
        return [{
            xtype: "fieldset",
            title: "Information",
            defaults: {
                labelSeparator: ""
            },
            items: [{
                xtype: "textfield",
                name: "root",
                fieldLabel: _("OMV Root"),
                submitValue: false,
                readOnly: true
            },{
                xtype: "textfield",
                name: "written",
                fieldLabel: _("Written kB since boot"),
                submitValue: false,
                readOnly: true
            }]
        },{
            xtype: "fieldset",
            title: _("Notes"),
            fieldDefaults: {
                labelSeparator: ""
            },
            items: [{
                border: false,
                html: "<p>" +
                         _("<b>The Flash Memory plugin works out of the box and moves most writes to the RAM without any further configuration needed.</b>") +
                         _("<b>Therefore the following instructions are 100% optional!  However, they allow to decrease the writes slightly more by disabling and removing the swap partition - recommended for advanced users only, and with a decent amount of RAM installed.</b>") +
                         _("Fstab (/etc/fstab) needs to be changed manually. Follow these steps:") +
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
                             "</li>" +
                          "<li>" + _("Ctrl-o to save") + "</li>" +
                          "<li>" + _("Ctrl-x to exit") + "</li>" +
                          "<li>" + _("If you disable swap, initramfs resume should be fixed to avoid mdadm messages.") + "</li>" +
                             "<dl>" +
                               "<dt>" + _("Remove the resume file:") + "</dt>" +
                               "<dd>" + "<b>rm /etc/initramfs-tools/conf.d/resume</b>" + "</dd>" +
                               "<dt>" + _("Update initramfs:") + "</dt>" +
                               "<dd>" + "<b>update-initramfs -u</b>" + "</dd>" +
                             "</dl>" +
                             "</li>" +
                          "<li>" + _("On systems using GPT, swap will still be auto-detected.  The swap partition should be turned off and wiped.") + "</li>" +
                             "<dl>" +
                               "<dt>" + _("Find the swap partition with:") + "</dt>" +
                               "<dd>" + "<b>blkid | grep swap</b>" + "</dd>" +
                               "<dt>" + _("Example output where /dev/sda2 is the swap partition:") + "</dt>" +
                               "<dd>" + '/dev/sda2: UUID="01c2d1ab-354b-4e2b-8d7c-ca35793f5fe7" TYPE="swap" PARTUUID="4f64854c-02"' + "</dd>" +
                               "<dt>" + _("Disable the swap partition with swapoff:") + "</dt>" +
                               "<dd>" + "<b>swapoff /dev/sda2</b>" + "</dd>" +
                               "<dt>" + _("Wipe the swap partition with wipefs:") + "</dt>" +
                               "<dd>" + "<b>wipefs -a /dev/sda2</b>" + "</dd>" +
                             "</dl>" +
                             "</li>" +
                          "<li>" + _("Reboot") + "</li>" +
                         "</ol>" +
                         "</p>"
            }]
        }];
    }
});

OMV.WorkspaceManager.registerPanel({
    id: "settings",
    path: "/storage/flashmemory",
    text: _("Settings"),
    position: 10,
    className: "OMV.module.admin.storage.flashmemory.Settings"
});
