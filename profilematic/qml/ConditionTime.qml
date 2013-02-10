/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
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
    tools: timeTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property RuleCondition condition // : (rule !== null ? rule.condition : null)

    ToolBarLayout {
        id: timeTools
        visible: false
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: isValidTimeRule() ? pageStack.pop() : dInvalidRule.open()
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
                text: "Time condition"
            }

            RuleTopicSummary {
                topic: "Weekdays"
                summary: daysSummary()
                showComboBox: true
                onTopicClicked: daysEditHandler()
            }

            RuleTopicSummary {
                topic: "Start time"
                summary: timeStartSummary()
                showComboBox: true
                onTopicClicked: timeStartEditHandler()
            }

            RuleTopicSummary {
                topic: "End time"
                summary: timeEndSummary()
                showComboBox: true
                onTopicClicked: timeEndEditHandler()
            }

            Button {
                text: "Clear"
                onClicked: dConfirmDelete.open()
            }

            LabelHelp {
                id: timeSummary
                text: root.timeSummary()
                Connections {
                    target: condition
                    onDaysChanged:      timeSummary.text = root.timeSummary()
                    onTimeStartChanged: timeSummary.text = root.timeSummary()
                    onTimeEndChanged:   timeSummary.text = root.timeSummary()
                }
            }
        }
    }

    function isValidTimeRule() {
        if (condition.timeStart === "" && condition.timeEnd === "" && condition.days.length === 0)
            return true
        if (condition.timeStart !== "" && condition.timeEnd !== "" && condition.days.length > 0)
            return true
        return false
    }

    QueryDialog {
        id: dConfirmDelete

        titleText: "Clear time conditions"
        message: "Are you sure you want to clear time conditions?"
        acceptButtonText: "Yes"
        rejectButtonText: "Cancel"

        onAccepted: {
            condition.timeStart = ""
            condition.timeEnd = ""
            condition.days = []
        }
    }

    QueryDialog {
        id: dInvalidRule

        titleText: "Invalid time condition"
        message: "Either set all time and day fields, or clear them.\n\n"
                 + "Missing:\n\n"
                 + (condition.timeStart   === '' ? "Start time\n" : "")
                 + (condition.timeEnd     === '' ? "End time\n"   : "")
                 + (condition.days.length === 0  ? "Weekdays"     : "")
        acceptButtonText: "Ok"
    }

    // Profile functions
    function timeSummary() {
        return backendRulesModel.getTimeSummaryText(condition, "") === "" ? "All time and day fields must be set. This condition is not yet usable." : "";
    }

    function formatTime(hour, minute) {
        return (hour < 10 ? "0" : "") + hour + ":" + (minute < 10 ? "0" : "") + minute
    }

    TimeDialog {
        id: timeStartDialog
        titleText: "Start time"
        onAccepted: condition.timeStart = formatTime(hour, minute)
    }

    // Start time functions
    function timeStartEditHandler() {
        var time = (condition.timeStart !== "" ? condition.timeStart : "00:00")
        var timeSplits = time.split(":")

        timeStartDialog.hour = timeSplits[0]
        timeStartDialog.minute = timeSplits[1]

        timeStartDialog.open();
    }

    function timeStartSummary() {
        if (condition.timeStart === '') {
            return "Click to set time"
        }
        return condition.timeStart
    }

    TimeDialog {
        id: timeEndDialog
        titleText: "End time"
        onAccepted: condition.timeEnd = formatTime(hour, minute)
    }

    // Start time functions
    function timeEndEditHandler() {
        var time = (condition.timeEnd !== "" ? condition.timeEnd : "00:00")
        var timeSplits = time.split(":")

        timeEndDialog.hour = timeSplits[0]
        timeEndDialog.minute = timeSplits[1]

        timeEndDialog.open();
    }

    function timeEndSummary() {
        if (condition.timeEnd=== '') {
            return "Click to set time"
        }
        return condition.timeEnd
    }


    MyMultiSelectionDialog {
        id: daysDialog
        titleText: "Weekdays"
        platformStyle: SelectionDialogStyle {
            itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
        }
        model: backendDaysModel
        acceptButtonText: "OK"
        onAccepted: condition.days = selectedIndexes
    }

    function daysSummary() {
        return backendRulesModel.getDaysSummaryText(condition.days);
    }

    function daysEditHandler() {
        daysDialog.selectedIndexes = condition.days
        daysDialog.open();
    }

}
