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
import Rule 1.0
import "UIConstants.js" as UIConstants

Page {
    id: root
    tools: commonTools
    anchors.leftMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin
    anchors.rightMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin

    property RuleCondition condition;
    property int maxCells: 50

    Flickable {
        anchors.fill: parent
        // anchors.topMargin: header.height
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
                text: qsTr("Idle condition")
            }

            LabelHelp {
                text: qsTr("Specify the amount of time in minutes that the device has to be in idle mode until this "
                      + "rule is activated. Device goes into idle if user does not do anything for a while (about 30 seconds). "
                      + "Idle mode is exited when lock screen is deactivated.")
            }

            RuleTopicSummary {
                id: summary
                topic: qsTr("Idle in minutes")
                summary: idleSummary()
                showComboBox: true
                onTopicClicked: idleEditHandler()
            }

            Connections {
                target: condition
                onIdleForSecsChanged: {
                    console.debug("ConditionIdleForSecs onChanged")
                    summary.summary = idleSummary()
                }

            }

            Button {
                text: qsTr("Clear")
                onClicked: {
                    backendRuleUtil.idleClear(condition)
                }
            }

        } // Column
    } // Flickable

    TimeDialog {
        id: idleDialog
        titleText: qsTr("Idle time")
        onAccepted: {
            condition.idleForSecs = (hour * 60 + minute) * 60
        }
    }

    function idleEditHandler() {
        if (condition.idleForSecs > 0) {
            idleDialog.hour = Math.floor(condition.idleForSecs / 3600)
            idleDialog.minute = (condition.idleForSecs / 60) % 60
        } else {
            idleDialog.hour = 0
            idleDialog.minute = 0
        }

        idleDialog.open();
    }

    function idleSummary() {
        return backendRuleUtil.idleSummary(condition, qsTr("Not used"), false);
    }
}
