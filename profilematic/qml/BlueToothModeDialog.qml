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
import profilematic 1.0

import "UIConstants.js" as UIConstants

MySelectionDialog {
    id: root
    titleText: "Set Bluetooth mode"
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
        if (status === DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < blueToothModel.count; i++) {
            var blueToothMode = blueToothModel.get(i).mode
            if (selectedBlueToothMode === blueToothMode) {
                selectedIndex = i
            }
        }
        open()
    }

//    ChoiseModel {
//        id: blueToothModel;

//        Component.onCompleted: {
//            add(0, qsTr("Off"), qsTr("Bluetooth off"))
//            add(1, qsTr("On"), qsTr("Bluetooth on"))
//            add(2, qsTr("On and visible"), qsTr("Bluetooth on and visible"))
//            add(-1, qsTr("Don't change"), qsTr("Don't change"))
//        }
//    }

    // BlueTooth mode
    ListModel {
        id: blueToothModel;

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
                addElement(0,  qsTr("Off"),            qsTr("Bluetooth off"))
                addElement(1,  qsTr("On"),             qsTr("Bluetooth on"))
                addElement(2,  qsTr("On and visible"), qsTr("Bluetooth on and visible"))
                addElement(-1, qsTr("Don't change"),   qsTr("Don't change"))
            }
        }

        Component.onCompleted: {
            initialize()
        }
    }

    function blueToothModeToText(mode) {
        // This function seems to be able to be called before ListModel's onCompleted,
        // so try initialize here also.
        blueToothModel.initialize()

        switch (mode) {
        case 0:
        case 1:
        case 2:
            return blueToothModel.get(mode).description;
        }
        return blueToothModel.get(3).description;
    }
}
