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
    titleText: qsTr("Internet connection mode")
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
                addElement(0,  qsTr("Not in use"),             qsTr("Not in use"))
                addElement(1,  qsTr("WLAN"),                   qsTr("When using internet over WLAN connection"))
                addElement(2,  qsTr("Mobile"),                 qsTr("When using internet over mobile connection"))
                addElement(3,  qsTr("Any"),                    qsTr("When any internet connection is in use"))
                addElement(4,  qsTr("No internet connection"), qsTr("When internet connection is disabled or unavailable"))
            }
        }

        Component.onCompleted: {
            initialize()
        }

    }

    function internetConnectionModeSummary(internetConnectionMode) {
        internetConnectionModeModel.initialize()

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
