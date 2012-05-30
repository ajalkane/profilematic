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
    titleText: "Match network mode"
    model: networkModeModel
    platformStyle: SelectionDialogStyle {
       itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
    }

    property int selectedNetworkMode: 0
    signal networkModeSelected

    onSelectedIndexChanged: {
        if (selectedIndex > -1) {
            selectedNetworkMode = model.get(selectedIndex).mode
            networkModeSelected()
        }
    }

    onStatusChanged: {
        if (status === DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < networkModeModel.count; i++) {
            var networkMode = networkModeModel.get(i).mode
            if (selectedNetworkMode === networkMode) {
                selectedIndex = i
            }
        }
        open()
    }

    // Flight mode
    ListModel {
        id: networkModeModel;

        ListElement{
            mode: 0
            name: "Not in use"
            description: "Not in use"
        }
        ListElement {
            mode: 1
            name: "WLAN"
            description: "When using internet over WLAN connection"
        }
        ListElement {
            mode: 2
            name: "GSM"
            description: "When using internet over mobile connection"
        }
    }

    function networkModeSummary(networkMode) {
        switch (networkMode) {
        case 1:
        case 2:
            return networkModeModel.get(networkMode).description;
        }
        return networkModeModel.get(0).description;
    }
}
