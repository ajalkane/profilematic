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

Item {
    property alias labelText: headerLabel.text
    property alias placeholderText: labelField.placeholderText
    property alias text: labelField.text
    property alias inputMask: labelField.inputMask
    property alias inputMethodHints: labelField.inputMethodHints
    property alias validator: labelField.validator

    height: childrenRect.height

    signal accepted

    Text {
        id: headerLabel
        anchors.rightMargin: 8 // IMPROVE UIConstants
        // anchors.bottomMargin: UIConstants.PADDING_SMALL
        font.pixelSize: UIConstants.FONT_LSMALL
        font.weight: Font.Light

        color: theme.inverted ? UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND : UIConstants.COLOR_SECONDARY_FOREGROUND
    }

    TextField {
        id: labelField
        anchors.top: headerLabel.bottom
        anchors.rightMargin: headerLabel.anchors.rightMargin
        width: parent.width
        Keys.onReturnPressed: {
            platformCloseSoftwareInputPanel()
            dummy.focus = true
        }
    }
    Item { id: dummy }

    Component.onCompleted: labelField.accepted.connect(accepted)
}
