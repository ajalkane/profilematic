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

import Rule 1.0

import "UIConstants.js" as UIConstants

Item {
    id: root
    // TODO temporary
    property bool ruleVisible: true
    property alias topic: topic.text
    property alias summary: summary.text

    signal topicClicked

    width: parent.width
    height: content.height

    Rectangle {
        id: background
        anchors.fill:  root
        anchors.margins: -UIConstants.DEFAULT_MARGIN
        visible: mouseArea.pressed
        color: UIConstants.COLOR_SELECT
    }

    Column {
        id: content
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter

        Label {
            id: topic
            platformStyle: LabelStyleTitle {}
            font.weight: Font.Bold
            // font: UiConstants.TitleFont
        }
        Label {
            id: summary
            width: parent.width
            platformStyle: LabelStyleSubtitle {}
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: background

        onClicked: {
            topicClicked()
        }
    }
}

