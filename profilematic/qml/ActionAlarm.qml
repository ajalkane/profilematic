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

    property RuleAction action;

    // TODO: should be JS library, used at least in ConditionCalendarAdvanced and ActionAlarmAdvanced and here
    function timeInSecondsAdjustSummary(adjustInSeconds) {
        var min = Math.floor(adjustInSeconds / 60)
        var sec = adjustInSeconds % 60

        if (min === 0) {
            return qsTr("%1 seconds").arg(sec)
        }
        if (sec === 0) {
            return qsTr("%1 minutes").arg(min)
        }
        return qsTr("%1 minutes %2 seconds").arg(min).arg(sec)
    }

    function alarmDelayEditHandler() {
        if (action.alarm.alarmInSeconds >= 0) {
            alarmDelayDialog.minute = Math.floor(action.alarm.alarmInSeconds / 60)
            alarmDelayDialog.second = action.alarm.alarmInSeconds  % 60
        } else {
            alarmDelayDialog.minute = 0
            alarmDelayDialog.second = 0
        }

        alarmDelayDialog.open();
    }


    TimeInMinutesAndSecondsDialog {
        id: alarmDelayDialog
        titleText: qsTr("Alarm delay before shown")
        onAccepted: {
            action.alarm.alarmInSeconds = minute * 60 + second
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
            spacing: UIConstants.PADDING_XXLARGE
            anchors.verticalCenter: parent.verticalCenter

            width: parent.width
            height: childrenRect.height

            PageHeader {
                text: qsTr("Alarm action")
            }

            LabelHelp {
                text: qsTr("Schedule alarm to be shown after specified time with title of your choosing. Both title and alarm must be set for alarm to be used.")
            }

            TextFieldWithLabel {
                labelText: qsTr("Alarm title")
                placeholderText: qsTr("Set alarm title")
                text: action.alarm.title
                width: parent.width
                maximumLength: 70
                onTextChanged: {
                    action.alarm.title = text
                }
            }

            RuleTopicSummary {
                topic: qsTr("Alarm delay before shown")
                summary: action.alarm.alarmInSeconds >=0 ? timeInSecondsAdjustSummary(action.alarm.alarmInSeconds) : "Not set"
                showComboBox: true
                onTopicClicked: alarmDelayEditHandler()
            }

            Separator {}

            RuleTopic {
                topic: qsTr("Advanced settings")
                showDrillDown: true
                disabled: !action.alarm.isValid()
                onTopicClicked: {
                    root.pageStack.push(Qt.resolvedUrl("ActionAlarmAdvanced.qml"), { 'action': action });
                }
            }

            Separator {}

            LabelHelp {
                id: summary
                text: backendRuleUtil.alarmSummary(action, qsTr("Alarm action is not currently usable"), false)
                Connections {
                    target: action !== null ? action.alarm : null
                    onChanged: summary.text = backendRuleUtil.alarmSummary(action, qsTr("Alarm action is not currently usable"), false)
                }

            }                        
        }
    }
}
