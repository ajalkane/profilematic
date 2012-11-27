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
            condition.calendar.summaryMatch = keywords.text
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
                id: keywords
                labelText: "Keywords in calendar summary"
                placeholderText: "Not set"
                text: condition.calendar.summaryMatch
                width: parent.width
//                onTextChanged: {
//                    if (root.status === PageStatus.Active) {
//                        condition.calendar.summaryMatch = text
//                    }
//                }
            }

            LabelHelp {
                id: timeSummary
                text: "Keywords are space separated words that will be matched in the calendar's summary (topic)"
            }
        }
    }
}
