/*********************************************************************
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
import com.nokia.extras 1.0

Item {
    id: root

    property alias topic: topicSummary.topic
    property alias summary: topicSummary.summary
    property alias checked: topicSwitch.checked

    width: parent.width
    height: topicSummary.height

    RuleTopicSummary {
        id: topicSummary
        topicHeight: Math.max(topicImplicitHeight, topicSwitch.height)
        topicWidth: parent.width - topicSwitch.width
        onTopicClicked: topicSwitch.checked = !topicSwitch.checked
    }
    Switch {
        id: topicSwitch
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.verticalCenter: parent.top
        onCheckedChanged: root.checked = topicSwitch.checked
    }
}
