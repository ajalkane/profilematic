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
    titleText: "Set stand-by-screen mode"
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
        if (status == DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < standByScreenModel.count; i++) {
            var standByScreenMode = standByScreenModel.get(i).mode
            if (selectedStandByScreenMode == standByScreenMode) {
                selectedIndex = i
            }
        }
        open()
    }

    // StandByScreen mode
    ListModel {
        id: standByScreenModel;

        ListElement{
            mode: 0
            name: "Disable"
            description: "Stand-by-screen disabled"
        }
        ListElement {
            mode: 1
            name: "Enable"
            description: "Stand-by-screen enabled"
        }
        ListElement {
            mode: -1
            name: "Don't change"
            description: "Don't change"
        }
    }

    function standByScreenModeToText(mode) {
        switch (mode) {
        case 0:
        case 1:
            return standByScreenModel.get(mode).description;
        }
        return standByScreenModel.get(2).description;
    }
}
