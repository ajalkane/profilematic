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
    titleText: "Set background connections"
    model: backgroundConnectionsModel
    platformStyle: SelectionDialogStyle {
       itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
    }

    property int selectedBackgroundConnectionsMode: -1
    signal backgroundConnectionsModeSelected

    onSelectedIndexChanged: {
        console.log("BackgroundConnectionsModel onSelectedIndex changed", selectedIndex)
        if (selectedIndex > -1) {
            selectedBackgroundConnectionsMode = model.get(selectedIndex).mode
            backgroundConnectionsModeSelected()
        }
    }

    onStatusChanged: {
        if (status === DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < backgroundConnectionsModel.count; i++) {
            var backgroundConnectionsMode = backgroundConnectionsModel.get(i).mode
            if (selectedBackgroundConnectionsMode === backgroundConnectionsMode) {
                selectedIndex = i
            }
        }
        open()
    }

    // BackgroundConnections mode
    ListModel {
        id: backgroundConnectionsModel;

        ListElement{
            mode: 0
            name: "Disable"
            description: "Background connections disabled"
        }
        ListElement {
            mode: 1
            name: "Enable"
            description: "Background connections enabled"
        }
        ListElement {
            mode: -1
            name: "Don't change"
            description: "Don't change"
        }
    }

    function backgroundConnectionsModeToText(mode) {
        switch (mode) {
        case 0:
        case 1:
            return backgroundConnectionsModel.get(mode).description;
        }
        return backgroundConnectionsModel.get(2).description;
    }
}
