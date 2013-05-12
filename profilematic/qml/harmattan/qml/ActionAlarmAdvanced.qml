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
    anchors.leftMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin
    anchors.rightMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin

    property RuleAction action

    function timeInMinutesAdjustSummary(adjustInMinutes) {
        var hour = Math.floor(adjustInMinutes / 60)
        var min = adjustInMinutes % 60

        if (hour === 0) {
            return qsTr("%1 minutes").arg(min)
        }
        if (min === 0) {
            return qsTr("%1 hours").arg(hour)
        }
        return qsTr("%1 hours %2 minutes").arg(hour).arg(min)
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
        titleText: qsTr("Alarm snooze time")
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
                text: qsTr("Alarm action advanced")
            }

            RuleTopicSummary {
                topic: qsTr("Alarm snooze time")
                summary: action.alarm.snoozeInMinutes > 0 ? timeInMinutesAdjustSummary(action.alarm.snoozeInMinutes) : qsTr("Default snooze")
                showComboBox: true
                onTopicClicked: snoozeEditHandler()
            }

            TextFieldWithLabel {
                labelText: qsTr("Alarm sound file (full path)")
                placeholderText: qsTr("Default sound")
                text: action.alarm.sound
                width: parent.width
                onTextChanged: {
                    if (root.status === PageStatus.Active) {
                        action.alarm.sound = text
                    }
                }
            }

            LabelHelp {
                text: qsTr("Hint: ringtones reside in %1. You can use a file manager of your choise to find the exact file name to use for sound.").arg("/usr/share/sounds/ring-tones")
            }
        }
    }
}
