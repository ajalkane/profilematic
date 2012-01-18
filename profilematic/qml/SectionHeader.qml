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

/**
 * Can be used with section.delegate in ListView to display Harmattan style section headers.
 *
 * Requires the text to be available as property 'section'
 */
Item {
    width: parent.width
    height: 40 // TODO UIConstants

    property alias section: headerLabel.text

    Text {
        id: headerLabel
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 8 // IMPROVE UIConstants
        anchors.bottomMargin: 2 // IMPROVE UIConstants
        font.bold: true
        font.pointSize: 14 // 18 // IMPROVE UIConstants

        color: theme.inverted ? UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND : UIConstants.COLOR_SECONDARY_FOREGROUND
    }
    Image {
        anchors.right: headerLabel.left
        anchors.left: parent.left
        anchors.verticalCenter: headerLabel.verticalCenter
        anchors.rightMargin: 24 // IMPROVE UIConstants
        source: "image://theme/meegotouch-groupheader" + (theme.inverted ? "-inverted" : "") + "-background"
    }
}
