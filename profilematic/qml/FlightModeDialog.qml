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
    titleText: "Set flight mode"
    model: flightModel
    platformStyle: SelectionDialogStyle {
       itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
    }

    property int selectedFlightMode: -1
    signal flightModeSelected

    onSelectedIndexChanged: {
        if (selectedIndex > -1) {
            selectedFlightMode = model.get(selectedIndex).mode
            flightModeSelected()
        }
    }

    onStatusChanged: {
        if (status === DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < flightModel.count; i++) {
            var flightMode = flightModel.get(i).mode
            if (selectedFlightMode === flightMode) {
                selectedIndex = i
            }
        }
        open()
    }

    // Flight mode
    ListModel {
        id: flightModel;

        ListElement{
            mode: 0
            name: "Off"
            description: "Set flight mode off"
        }
        ListElement {
            mode: 1
            name: "On"
            description: "Set flight mode on"
        }
        // flightModelSummary depends on "Don't change" to be be index 2.
        ListElement {
            mode: -1
            name: "Don't change"
            description: "Don't change"
        }
    }

    function flightModeToText(flightMode) {
        switch (flightMode) {
        case 0:
        case 1:
            return flightModel.get(flightMode).description;
        }
        return flightModel.get(2).description;
    }
}
