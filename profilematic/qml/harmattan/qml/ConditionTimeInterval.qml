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
    tools: timeIntervalTools
    anchors.leftMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin
    anchors.rightMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin

    property RuleCondition condition;

    ToolBarLayout {
        id: timeIntervalTools
        visible: false
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: isValidTimeRule() ? pageStack.pop() : dInvalidRule.open()
        }
    }

    function isValidTimeRule() {
        if (condition.timeInterval.activeForSecs <= 0 && condition.timeInterval.inactiveForSecs <= 0) {
            return true;
        }

        return condition.timeInterval.isValid();
    }

    QueryDialog {
        id: dInvalidRule

        titleText: qsTr("Invalid time interval condition")
        message: qsTr("Both active and inactive duration must be set as at least 1 minute, or you can clear them.")
                 + "\n\n"
                 + qsTr("Invalid values:")
                 + "\n\n"
                 + (condition.timeInterval.activeForSecs <= 0 ? qsTr("Active duration") + "\n" : "")
                 + (condition.timeInterval.inactiveForSecs <= 0 ? qsTr("Inactive duration") + "\n" : "")
        acceptButtonText: "Ok"
    }

    QueryDialog {
        id: dConfirmClear

        titleText: qsTr("Clear time interval conditions")
        message: qsTr("Are you sure you want to clear time interval conditions?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("Cancel")

        onAccepted: backendRuleUtil.timeIntervalClear(condition)
    }

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
                text: qsTr("Time interval")
            }

            RuleTopicSummary {
                id: activeSummaryId
                topic: qsTr("Active duration")
                summary: activeSummary()
                showComboBox: true
                onTopicClicked: activeEditHandler()
            }

            RuleTopicSummary {
                id: inactiveSummaryId
                topic: qsTr("Inactive duration")
                summary: inactiveSummary()
                showComboBox: true
                onTopicClicked: inactiveEditHandler()
            }

            Connections {
                target: condition.timeInterval
                onInactiveForSecsChanged: inactiveSummaryId.summary = inactiveSummary()
                onActiveForSecsChanged: activeSummaryId.summary = activeSummary()
            }

            Button {
                text: qsTr("Clear")
                onClicked: dConfirmClear.open()
            }

            LabelHelp {
                text: qsTr("Can be used to activate some actions periodically. Active duration specifies the time period that the rule is active, "
                         + "inactive duration specifies the time period the rule is inactive after being active. The rule alternates between active "
                         + "and inactive states according to these parameters.")
            }

            LabelHelp {
                text: qsTr("Tip: You can add for example 'Time condition' with this rule to specify that the time interval is only active within specified time/days.")
            }


        } // Column
    } // Flickable

    TimeDialog {
        id: activeDialog
        titleText: qsTr("Active duration")
        onAccepted: {
            condition.timeInterval.activeForSecs = (hour * 60 + minute) * 60
        }
    }

    TimeDialog {
        id: inactiveDialog
        titleText: qsTr("Inactive duration")
        onAccepted: {
            condition.timeInterval.inactiveForSecs = (hour * 60 + minute) * 60
        }
    }

    function initializeTimePickerFromSecs(timePicker, secs) {
        if (secs > 0) {
            timePicker.hour = Math.floor(secs / 3600)
            timePicker.minute = (secs / 60) % 60
        } else {
            timePicker.hour = 0
            timePicker.minute = 0
        }

    }

    function activeEditHandler() {
        initializeTimePickerFromSecs(activeDialog, condition.timeInterval.activeForSecs)
        activeDialog.open();
    }

    function activeSummary() {
        return backendRuleUtil.timeIntervalActiveSummary(condition, qsTr("Not set"), false);
    }

    function inactiveEditHandler() {
        initializeTimePickerFromSecs(inactiveDialog, condition.timeInterval.inactiveForSecs)
        inactiveDialog.open();
    }

    function inactiveSummary() {
        return backendRuleUtil.timeIntervalInactiveSummary(condition, qsTr("Not set"), false);
    }

}
