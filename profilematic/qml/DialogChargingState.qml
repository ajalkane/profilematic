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
    titleText: qsTr("Charging state")
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
                addElement(-1, qsTr("Not in use"),   qsTr("Not in use"))
                addElement(0,  qsTr("Not charging"), qsTr("When device is not charging power"))
                addElement(1,  qsTr("Charging"),     qsTr("When device is charging power"))
            }
        }

        Component.onCompleted: {
            initialize()
        }
    }

    function chargingStateSummary(chargingState) {
        chargingStateModel.initialize();

        switch (chargingState) {
        case 0:
        case 1:
            return chargingStateModel.get(chargingState + 1).description;
        }
        return chargingStateModel.get(0).description;
    }
}
