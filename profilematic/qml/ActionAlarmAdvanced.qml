/**********************************************************************
 * Copyright 2013 Arto Jalkanen
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

Page {
    id: root
    tools: commonTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property RuleAction action

    function timeInMinutesAdjustSummary(adjustInMinutes) {
        var hour = Math.floor(adjustInMinutes / 60)
        var min = adjustInMinutes % 60

        if (hour === 0) {
            return min + " minutes"
        }
        if (min === 0) {
            return hour + " hours"
        }
        return hour + " hours "+min+" minutes";
    }

    function snoozeEditHandler() {
        if (action.alarm.snoozeInMinutes > 0) {
            snoozeDialog.hour = Math.floor(action.alarm.snoozeInMinutes / 60)
            snoozeDialog.minute = action.alarm.snoozeInMinutes % 60
        } else {
            snoozeDialog.hour = 0
            snoozeDialog.minute = 0
        }

        snoozeDialog.open();
    }

    TimeDialog {
        id: snoozeDialog
        titleText: "Alarm snooze time"
        onAccepted: {
            action.alarm.snoozeInMinutes = hour * 60 + minute
        }
    }

    Flickable {
        anchors.fill: parent
        pressDelay: 140
        clip: true
        contentWidth: parent.width
        contentHeight: container.height // contentItem.childrenRect.height

        Column {
            id: container
            spacing: UIConstants.PADDING_XLARGE
            anchors.verticalCenter: parent.verticalCenter

            width: parent.width
            height: childrenRect.height

            PageHeader {
                text: "Alarm action advanced"
            }

            RuleTopicSummary {
                topic: "Alarm snooze time"
                summary: action.alarm.snoozeInMinutes > 0 ? timeInMinutesAdjustSummary(action.alarm.snoozeInMinutes) : "Default snooze"
                showComboBox: true
                onTopicClicked: snoozeEditHandler()
            }

            TextFieldWithLabel {
                labelText: "Alarm sound file (full path)"
                placeholderText: "Default sound"
                text: action.alarm.sound
                width: parent.width
                onTextChanged: {
                    if (root.status === PageStatus.Active) {
                        action.alarm.sound = text
                    }
                }
            }
        }
    }
}
