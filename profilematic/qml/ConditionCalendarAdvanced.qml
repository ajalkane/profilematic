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
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property RuleCondition condition // : (rule !== null ? rule.condition : null)

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
                text: qsTr("Calendar condition advanced")
            }

            RuleTopicSummary {
                topic: qsTr("Match before calendar entry start")
                summary: timeCalendarAdjustSummary(condition.calendar.timePrepend)
                showComboBox: true
                onTopicClicked: timeCalendarBeforeEditHandler()
            }


            RuleTopicSummary {
                topic: qsTr("Match after calendar entry end")
                summary: timeCalendarAdjustSummary(condition.calendar.timeAppend)
                showComboBox: true
                onTopicClicked: timeCalendarAfterEditHandler()
            }
        }
    }

    function timeCalendarBeforeEditHandler() {
        timeBeforeDialog.minute = Math.floor(condition.calendar.timePrepend / 60)
        timeBeforeDialog.second = condition.calendar.timePrepend % 60

        timeBeforeDialog.open();
    }

    function timeCalendarAfterEditHandler() {
        timeBeforeDialog.minute = Math.floor(condition.calendar.timePrepend / 60)
        timeBeforeDialog.second = condition.calendar.timePrepend % 60

        timeAfterDialog.open();
    }

    function timeCalendarAdjustSummary(adjustInSeconds) {
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

    TimeInMinutesAndSecondsDialog {
        id: timeBeforeDialog
        titleText: qsTr("Before calendar entry")
        onAccepted: {
            condition.calendar.timePrepend = minute * 60 + second
        }
    }

    TimeInMinutesAndSecondsDialog {
        id: timeAfterDialog
        titleText: qsTr("After calendar entry")
        onAccepted: {
            condition.calendar.timeAppend = minute * 60 + second
        }
    }

}
