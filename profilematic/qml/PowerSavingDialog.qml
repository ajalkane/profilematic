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
    titleText: "Set power saving mode"
    model: powerSavingModel
    platformStyle: SelectionDialogStyle {
       itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
    }

    property int selectedPowerSavingMode: -1
    signal powerSavingModeSelected

    onSelectedIndexChanged: {
        if (selectedIndex > -1) {
            selectedPowerSavingMode = model.get(selectedIndex).mode
            powerSavingModeSelected()
        }
    }

    onStatusChanged: {
        if (status === DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < powerSavingModel.count; i++) {
            var powerSavingMode = powerSavingModel.get(i).mode
            if (selectedPowerSavingMode == powerSavingMode) {
                selectedIndex = i
            }
        }
        open()
    }

    ListModel {
        id: powerSavingModel;

        ListElement{
            mode: 0
            name: "Off"
            description: "Set power saving off"
        }
        ListElement {
            mode: 1
            name: "On"
            description: "Set power saving on"
        }
        // powerSavingModelSummary depends on "Don't change" to be be index 2.
        ListElement {
            mode: -1
            name: "Don't change"
            description: "Don't change"
        }
    }

    function powerSavingModeToText(mode) {
        switch (mode) {
        case 0:
        case 1:
            return powerSavingModel.get(mode).description;
        }
        return powerSavingModel.get(2).description;
    }
}
