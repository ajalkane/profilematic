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
    tools: timeTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property RuleCondition condition // : (rule !== null ? rule.condition : null)

    onStatusChanged: {
        // console.log("Status changed ", status)
        if (status === PageStatus.Activating) {
//            backendNfc.monitorNfc(true)
//            if (!backendNfc.isAvailable()) {
//                console.log("Opening NFC not active warning dialog")
//                dNfcNotActive.open();
//            } else {
//                console.log("NFC is active, no warning dialog")
//            }
        } else if (status === PageStatus.Deactivating) {
            condition.calendar.summaryMatch = summaryMatch.text
            condition.calendar.locationMatch = locationMatch.text
            condition.calendar.timePrepend = parseInt(timePrepend.text)
            condition.calendar.timeAppend = parseInt(timeAppend.text)
        }
    }

    ToolBarLayout {
        id: timeTools
        visible: false
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    SectionHeader {
        id: header
        section: "Calendar condition"
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

            TextFieldWithLabel {
                id: summaryMatch
                labelText: "Match in calendar summary"
                placeholderText: "Not set"
                text: condition.calendar.summaryMatch
                width: parent.width
            }

            TextFieldWithLabel {
                id: locationMatch
                labelText: "Match in location"
                placeholderText: "Not set"
                text: condition.calendar.locationMatch
                width: parent.width
            }

            TextFieldWithLabel {
                id: timePrepend
                labelText: "Prepend in seconds"
                placeholderText: "No prepend"
                text: condition.calendar.timePrepend !== 0 ? condition.calendar.timePrepend: ""
                inputMethodHints: Qt.ImhDigitsOnly
                validator: RegExpValidator { regExp: /\d{0,3}/ }
                width: parent.width
            }

            TextFieldWithLabel {
                id: timeAppend
                labelText: "Append in seconds"
                placeholderText: "No append"
                text: condition.calendar.timeAppend !== 0 ? condition.calendar.timeAppend: ""
                inputMethodHints: Qt.ImhDigitsOnly
                validator: RegExpValidator { regExp: /\d{0,3}/ }
                width: parent.width
            }

            LabelHelp {
                id: timeSummary
                text: "Give several matches by separating matches with comma"
            }
        }
    }
}
