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
                text: qsTr("Calendar condition")
            }

            LabelHelp {
                text: qsTr("Keywords in the fields are used to match calendar entries. "
                      + "Condition matches a calendar entry if all fields set here match it. "
                      + "At least one field must be set for the condition to be usable."
                      + "\n\n"
                      + "Comma (,) is used to separate keywords in a field. Any keyword separated by comma "
                      + "that is found in the calendar field will match. Matching is case insensitive.")
            }

            Separator {}

            TextFieldWithLabel {
                id: summaryMatch
                labelText: qsTr("Match summary in calendar entry")
                placeholderText: qsTr("Not set")
                text: condition.calendar.summaryMatch
                width: parent.width
                onTextChanged: {
                    if (root.status === PageStatus.Active) {
                        condition.calendar.summaryMatch = text
                    }
                }
            }

            TextFieldWithLabel {
                id: locationMatch
                labelText: qsTr("Match location in calendar entry")
                placeholderText: qsTr("Not set")
                text: condition.calendar.locationMatch
                width: parent.width
                onTextChanged: {
                    if (root.status === PageStatus.Active) {
                        condition.calendar.locationMatch = text
                    }
                }
            }

            Separator {}

            RuleTopic {
                topic: qsTr("Advanced settings")
                showDrillDown: true
                disabled: !condition.calendar.isValid()
                onTopicClicked: {
                    root.pageStack.push(Qt.resolvedUrl("ConditionCalendarAdvanced.qml"), { 'condition': condition });
                }
            }

            Separator {}
        }
    }
}
