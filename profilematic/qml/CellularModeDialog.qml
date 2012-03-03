/**********************************************************************
 * Copyright 2012 Arto Jalkanen
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
    titleText: "Set cellular mode"
    model: cellularModel
    platformStyle: SelectionDialogStyle {
       itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
    }

    property int selectedCellularMode: -1
    signal cellularModeSelected

    onSelectedIndexChanged: {
        if (selectedIndex > -1) {
            selectedCellularMode = model.get(selectedIndex).mode
            cellularModeSelected()
        }
    }

    onStatusChanged: {
        if (status == DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < cellularModel.count; i++) {
            var cellularMode = cellularModel.get(i).mode
            if (selectedCellularMode == cellularMode) {
                selectedIndex = i
            }
        }
        open()
    }

    // cellular mode
    ListModel {
        id: cellularModel;

        ListElement{
            mode: 0
            name: "Dual"
            description: "Set network mode to Dual"
        }
        ListElement {
            mode: 1
            name: "GSM"
            description: "Set network mode to GSM"
        }
        ListElement {
            mode: 2
            name: "3G"
            description: "Set network mode to 3G"
        }
        // CellularModelSummary depends on "Don't change" to be be index 3.
        ListElement {
            mode: -1
            name: "Don't change"
            description: "Don't change"
        }
    }

    function cellularModeToText(mode) {
        switch (mode) {
        case 0:
        case 1:
        case 2:
            return cellularModel.get(mode).description;
        }
        return cellularModel.get(3).description;
    }
}
