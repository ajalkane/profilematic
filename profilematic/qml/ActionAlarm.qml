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

    property RuleAction action;

    Flickable {
        anchors.fill: parent
        pressDelay: 140
        clip: true
        contentWidth: parent.width
        contentHeight: container.height // contentItem.childrenRect.height

        Column {
            id: container
            spacing: UIConstants.PADDING_XXLARGE
            anchors.verticalCenter: parent.verticalCenter

            width: parent.width
            height: childrenRect.height

            PageHeader {
                text: "Alarm action"
            }

            LabelHelp {
                text: "Schedule alarm to be shown after specified time with title of your choosing. Both title and alarm must be set for alarm to be used."
            }

            TextFieldWithLabel {
                labelText: "Alarm title"
                placeholderText: "Set alarm title"
                text: action.alarm.title
                width: parent.width
                maximumLength: 70
                onTextChanged: {
                    action.alarm.title = text
                }
            }

            TextFieldWithLabel {
                labelText: "Alarm delay in seconds"
                placeholderText: "Not set"
                text: action.alarm.alarmInSeconds < 0 ? "" : action.alarm.alarmInSeconds
                inputMethodHints: Qt.ImhDigitsOnly
                validator: RegExpValidator { regExp: /\d{0,4}/ }
                width: parent.width
                onTextChanged: {
                    if (root.status === PageStatus.Active) {
                        action.alarm.alarmInSeconds = (text !== "" ? parseInt(text) : -1)
                    }
                }
            }

            LabelHelp {
                id: summary
                text: backendRuleUtil.alarmSummary(action, "Alarm action is not currently usable", false)
                Connections {
                    target: action !== null ? action.alarm : null
                    onChanged: summary.text = backendRuleUtil.alarmSummary(action, "Alarm action is not currently usable", false)
                }

            }
        }
    }
}
