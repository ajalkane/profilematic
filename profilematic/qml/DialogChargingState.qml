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
    titleText: "Charging state"
    model: chargingStateModel
    platformStyle: SelectionDialogStyle {
       itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
    }

    property int selectedChargingState: -1
    signal chargingStateSelected

    onSelectedIndexChanged: {
        console.log("ChargingState selectedIndexChanged", selectedIndex)
        if (selectedIndex > -1) {
            selectedChargingState = model.get(selectedIndex).mode
            chargingStateSelected()
        }
    }

    onStatusChanged: {
        if (status === DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < chargingStateModel.count; i++) {
            var chargingState = chargingStateModel.get(i).mode
            console.log("DialogChargingState.__selectIndex i: " + i + " chargingState: " + chargingState + " selectedChargingState: " + selectedChargingState)
            if (selectedChargingState === chargingState) {
                selectedIndex = i
            }
        }
        console.log("DialogChargingState.__selectIndex selectedIndex before open: " + selectedIndex);
        open()
    }

    ListModel {
        id: chargingStateModel;

        ListElement{
            mode: -1
            name: "Not in use"
            description: "Not in use"
        }
        ListElement {
            mode: 0
            name: "Not charging"
            description: "When device is not charging power"
        }
        ListElement {
            mode: 1
            name: "Charging"
            description: "When device is charging power"
        }
    }

    function chargingStateSummary(chargingState) {
        switch (chargingState) {
        case 0:
        case 1:
            return chargingStateModel.get(chargingState + 1).description;
        }
        return chargingStateModel.get(0).description;
    }
}
