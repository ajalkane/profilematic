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
    titleText: "Internet connection mode"
    model: internetConnectionModeModel
    platformStyle: SelectionDialogStyle {
       itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
    }

    property int selectedInternetConnectionMode: 0
    signal internetConnectionModeSelected

    onSelectedIndexChanged: {
        if (selectedIndex > -1) {
            selectedInternetConnectionMode = model.get(selectedIndex).mode
            internetConnectionModeSelected()
        }
    }

    onStatusChanged: {
        if (status === DialogStatus.Opening) {
            selectedIndex = -1
            __selectIndex()
        }
    }

    function __selectIndex() {
        for (var i = 0; i < internetConnectionModeModel.count; i++) {
            var internetConnectionMode = internetConnectionModeModel.get(i).mode
            if (selectedInternetConnectionMode === internetConnectionMode) {
                selectedIndex = i
            }
        }
        open()
    }

    // Flight mode
    ListModel {
        id: internetConnectionModeModel;

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
            name: "Mobile"
            description: "When using internet over mobile connection"
        }
        ListElement {
            mode: 3
            name: "Any"
            description: "When any internet connection is in use"
        }
        ListElement {
            mode: 4
            name: "No internet connection"
            description: "When no internet connection is in use"
        }
    }

    function internetConnectionModeSummary(internetConnectionMode) {
        switch (internetConnectionMode) {
        case 1:
        case 2:
        case 3:
        case 4:
            return internetConnectionModeModel.get(internetConnectionMode).description;
        }
        return internetConnectionModeModel.get(0).description;
    }
}
