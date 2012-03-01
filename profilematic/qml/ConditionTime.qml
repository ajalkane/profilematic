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

    property Rule    rule;

    ToolBarLayout {
        id: timeTools
        visible: false
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: isValidTimeRule() ? pageStack.pop() : dInvalidRule.open()
        }
    }

    SectionHeader {
        id: header
        section: "Choose time and weekdays"
    }

    Flickable {
        anchors.fill: parent
        anchors.topMargin: header.height
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

            RuleTopicSummary {
                topic: "Weekdays"
                summary: daysSummary()
                showComboBox: true
                onTopicClicked: daysEditHandler()
                visible: !rule.isDefaultRule
            }

            RuleTopicSummary {
                topic: "Start time"
                summary: timeStartSummary()
                showComboBox: true
                onTopicClicked: timeStartEditHandler()
                visible: !rule.isDefaultRule
            }

            RuleTopicSummary {
                topic: "End time"
                summary: timeEndSummary()
                showComboBox: true
                onTopicClicked: timeEndEditHandler()
                visible: !rule.isDefaultRule
            }

            Button {
                text: "Clear"
                onClicked: dConfirmDelete.open()
            }

            Text {
                id: timeSummary
                wrapMode: Text.WordWrap
                width: parent.width
                // visible: isValidRule()
                font.pixelSize: UIConstants.FONT_SMALL;
                color: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                text: root.timeSummary()
                Connections {
                    target: rule
                    onDaysChanged:      timeSummary.text = root.timeSummary()
                    onTimeStartChanged: timeSummary.text = root.timeSummary()
                    onTimeEndChanged:   timeSummary.text = root.timeSummary()
                }
            }
        }
    }

    function isValidTimeRule() {
        if (rule.timeStart === "" && rule.timeEnd === "" && rule.days.length === 0)
            return true
        if (rule.timeStart !== "" && rule.timeEnd !== "" && rule.days.length > 0)
            return true
        return false
    }

    QueryDialog {
        id: dConfirmDelete

        titleText: "Clear time rules"
        message: "Are you sure you want to clear time rules?"
        acceptButtonText: "Yes"
        rejectButtonText: "Cancel"

        onAccepted: {
            rule.timeStart = ""
            rule.timeEnd = ""
            rule.days = []
        }
    }

    QueryDialog {
        id: dInvalidRule

        titleText: "Invalid time rule"
        message: "Either set all time and day fields, or clear them.\n\n"
                 + "Missing:\n\n"
                 + (rule.timeStart   === '' ? "Start time\n" : "")
                 + (rule.timeEnd     === '' ? "End time\n"   : "")
                 + (rule.days.length === 0  ? "Weekdays"     : "")
        acceptButtonText: "Ok"
    }

    // Profile functions
    function timeSummary() {
        return backendRulesModel.getTimeSummaryText(rule, "") === "" ? "All time and day fields must be set. This condition is not yet usable." : "";
    }

    function formatTime(hour, minute) {
        return (hour < 10 ? "0" : "") + hour + ":" + (minute < 10 ? "0" : "") + minute
    }

    TimeDialog {
        id: timeStartDialog
        titleText: "Start time"
        onAccepted: rule.timeStart = formatTime(hour, minute)
    }

    // Start time functions
    function timeStartEditHandler() {
        var time = (rule.timeStart !== "" ? rule.timeStart : "00:00")
        var timeSplits = time.split(":")

        timeStartDialog.hour = timeSplits[0]
        timeStartDialog.minute = timeSplits[1]

        timeStartDialog.open();
    }

    function timeStartSummary() {
        if (rule.timeStart === '') {
            return "Click to set time"
        }
        return rule.timeStart
    }

    TimeDialog {
        id: timeEndDialog
        titleText: "End time"
        onAccepted: rule.timeEnd = formatTime(hour, minute)
    }

    // Start time functions
    function timeEndEditHandler() {
        var time = (rule.timeEnd !== "" ? rule.timeEnd : "00:00")
        var timeSplits = time.split(":")

        timeEndDialog.hour = timeSplits[0]
        timeEndDialog.minute = timeSplits[1]

        timeEndDialog.open();
    }

    function timeEndSummary() {
        if (rule.timeEnd=== '') {
            return "Click to set time"
        }
        return rule.timeEnd
    }


    MyMultiSelectionDialog {
        id: daysDialog
        titleText: "Weekdays"
        platformStyle: SelectionDialogStyle {
            itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
        }
        model: backendDaysModel
        acceptButtonText: "OK"
        onAccepted: rule.days = selectedIndexes
    }

    function daysSummary() {
        return backendRulesModel.getDaysSummaryText(rule.days);
    }

    function daysEditHandler() {
        daysDialog.selectedIndexes = rule.days
        daysDialog.open();
    }

}
