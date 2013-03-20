/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
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
import "UIConstants.js" as UIConstants

TumblerDialog {
    id: root
    titleText: qsTr("Battery level range")
    anchors.margins: UIConstants.DEFAULT_MARGIN
    property alias selectedMin: minColumn.selectedIndex
    property alias selectedMax: maxColumn.selectedIndex

    signal selected(int min, int max)

    ListModel {
        id: percentageList
        Component.onCompleted: {
            for (var pct = 0; pct <= 100; pct++) {
                percentageList.append({"value" : pct});
            }
        }
    }

    TumblerColumn {
        id: minColumn
        label: qsTr("Min")
        selectedIndex: 101
        items: percentageList
    }

    TumblerColumn {
        id: maxColumn
        label: qsTr("Max")
        selectedIndex: 0
        items: percentageList
    }

    columns: [ minColumn, maxColumn ]
    onAccepted: {
        selected(minColumn.selectedIndex, maxColumn.selectedIndex)
    }
}
