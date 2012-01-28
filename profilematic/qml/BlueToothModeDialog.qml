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
    titleText: "Set BlueTooth mode"
    model: blueToothModel
    platformStyle: SelectionDialogStyle {
       itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
    }

    property int selectedBlueToothMode: -1
    signal blueToothModeSelected

    onSelectedIndexChanged: {
        console.log("blueToothModel onSelectedIndex changed", selectedIndex)
        if (selectedIndex > -1) {
            selectedBlueToothMode = model.get(selectedIndex).mode
            blueToothModeSelected()
        }
    }

    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < blueToothModel.count; i++) {
            var blueToothMode = blueToothModel.get(i).mode
            if (selectedBlueToothMode == blueToothMode) {
                selectedIndex = i
            }
        }
        open()
    }

    // BlueTooth mode
    ListModel {
        id: blueToothModel;

        ListElement{
            mode: 0
            name: "BlueTooth off"
        }
        ListElement {
            mode: 1
            name: "BlueTooth on"
        }
        ListElement {
            mode: 2
            name: "BlueTooth on and visible"
        }
        // BlueToothModelSummary dpeends on "Don't change" to be be index 3.
        ListElement {
            mode: -1
            name: "Don't change"
        }
    }

    function blueToothModeToText(mode) {
        switch (mode) {
        case 0:
        case 1:
        case 2:
            return blueToothModel.get(mode).name;
        }
        return blueToothModel.get(3).name;
    }
}
