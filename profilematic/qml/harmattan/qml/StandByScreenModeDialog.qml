/**********************************************************************
 * Copyright 2011 Arto Jalkanen
 *
 * This file is part of ProfileMatic.
 *
 * ProfileMatic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProfileMatic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProfileMatic.  If not, see <http://www.gnu.org/licenses/>
**/
import QtQuick 1.1
import com.nokia.meego 1.0

import "UIConstants.js" as UIConstants

MySelectionDialog {
    id: root
    titleText: qsTr("Set stand-by-screen mode")
    model: standByScreenModel
    platformStyle: SelectionDialogStyle {
       itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
    }

    property int selectedStandByScreenMode: -1
    signal standByScreenModeSelected

    onSelectedIndexChanged: {
        console.log("StandByScreenModel onSelectedIndex changed", selectedIndex)
        if (selectedIndex > -1) {
            selectedStandByScreenMode = model.get(selectedIndex).mode
            standByScreenModeSelected()
        }
    }

    onStatusChanged: {
        if (status === DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < standByScreenModel.count; i++) {
            var standByScreenMode = standByScreenModel.get(i).mode
            if (selectedStandByScreenMode === standByScreenMode) {
                selectedIndex = i
            }
        }
        open()
    }

    // StandByScreen mode
    ListModel {
        id: standByScreenModel;

        function addElement(mode, name, description) {
            append({
                       "mode" : mode,
                       "name" : name,
                       "description" : description
                   })
        }


        function initialize() {
            // ListElement can't have JavaScript (ie. qsTr), so have to create the model like this
            if (count === 0) {
                addElement(0,  qsTr("Disable"),            qsTr("Stand-by-screen disabled"))
                addElement(1,  qsTr("Enable"),             qsTr("Stand-by-screen enabled"))
                addElement(-1, qsTr("Don't change"),   qsTr("Don't change"))
            }
        }

        Component.onCompleted: {
            initialize()
        }


    }

    function standByScreenModeToText(mode) {
        standByScreenModel.initialize();

        switch (mode) {
        case 0:
        case 1:
            return standByScreenModel.get(mode).description;
        }
        return standByScreenModel.get(2).description;
    }
}
