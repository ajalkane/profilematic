/*********************************************************************
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
import com.nokia.extras 1.0

import Rule 1.0
import profilematic 1.0

import "UIConstants.js" as UIConstants

Item {
    id: root
    property alias model: dDialog.model
    property alias titleText: dDialog.titleText
    property alias selectedIndex: dDialog.selectedIndex
    signal itemSelected(variant item)

    function open() {
        dDialog.open()
    }

    // This is a hack to access from SelectionDialog the roles of the model.
    ListView {
        id: lModel
        model: root.model
        delegate: Item {
            property variant myModel: model
        }
    }

    MySelectionDialog {
        id: dDialog
        platformStyle: SelectionDialogStyle {
            itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
        }
        onSelectedIndexChanged: {
            if (selectedIndex > -1) {
                console.log("Selected condition index", selectedIndex)
                // This is a hack to access from SelectionDialog the roles of the model.
                // Ugly but works.
                lModel.currentIndex = selectedIndex
                itemSelected(lModel.currentItem.myModel)
            }
        }
    }
}
