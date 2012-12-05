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

Item {
    width: parent.width
    height: childrenRect.height + UIConstants.PADDING_LARGE

    property alias text: headerText.text
    property alias separator: separator.visible

    Text {
        id: headerText
        anchors.rightMargin: 8 // IMPROVE UIConstants
        anchors.top: parent.top
        font.pixelSize: UIConstants.FONT_XLARGE
        font.weight: Font.Light

        color: theme.inverted ? UIConstants.COLOR_INVERTED_FOREGROUND : UIConstants.COLOR_FOREGROUND
    }
    Image {
        id: separator
        visible: true
        anchors.left: headerText.left
        anchors.right: parent.right
        anchors.top: headerText.bottom
        anchors.topMargin: UIConstants.PADDING_LARGE
        source: "image://theme/meegotouch-groupheader" + (theme.inverted ? "-inverted" : "") + "-background"
    }
}
