/**********************************************************************
 * Copyright 2011-2012 Philip Lorenz
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
import profilematic 1.0
import "UIConstants.js" as UIConstants

Page {
    id: root
    property Rule rule;

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                rule.presenceRules = listView.model.presenceRules
                pageStack.pop();
            }
        }
    }

    anchors.margins: UIConstants.DEFAULT_MARGIN

    SectionHeader {
        id: header
        section: "Set availability"
    }

    TextField {
        id: statusMessage

        anchors { top: header.bottom; left: parent.left; right: parent.right; topMargin: UIConstants.DEFAULT_MARGIN }
        height: visible ? statusMessage.implicitHeight : 0
        visible: {
            for (var row = 0; row < listView.model.presenceRules.length; row++) {
                if (listView.model.presenceRules[row].action == PresenceRule.SetOnline)
                    return true;
            }

            return false;
        }
        style: TextFieldStyle {
            paddingRight: clearButton.width
        }

        placeholderText: "Status message"
        text: rule.presenceStatusMessage

        Image {
            id: clearButton

            anchors.right: parent.right
            width: parent.height; height: parent.height

            source: "image://theme/icon-m-input-clear"
        }

        MouseArea {
            // Force ourself to have a higher Z-Index than the TextField's MouseArea
            z: 99
            anchors.right: statusMessage.right
            width: clearButton.width; height: clearButton.height

            onClicked: statusMessage.text = ""
        }

        Behavior on height { NumberAnimation { duration: 250 } }

        onTextChanged: rule.presenceStatusMessage = text
    }

    Row {
        id: buttonBar

        function __changeAllActions(to) {
            for (var row = 0; row < listView.model.presenceRules.length; row++) {
                listView.model.presenceRules[row].action = to;
            }
        }

        spacing: UIConstants.PADDING_SMALL
        anchors { top: statusMessage.bottom; right: parent.right; left: parent.left; topMargin: UIConstants.DEFAULT_MARGIN }
        visible: listView.count > 0

        Button {
            text: "All online"
            width: (parent.width - parent.spacing) / 2
            onClicked: buttonBar.__changeAllActions(PresenceRule.SetOnline)
        }

        Button {
            text: "All offline"
            width: (parent.width - parent.spacing) / 2
            onClicked: buttonBar.__changeAllActions(PresenceRule.SetOffline)
        }
    }

    ListView {
        id: listView

        clip: true
        anchors { top: buttonBar.bottom; right: parent.right; bottom: restorePresences.bottom; left: parent.left; topMargin: UIConstants.DEFAULT_MARGIN }
        model: AccountsModel {
            presenceRules: root.rule.presenceRules
        }

        delegate: Rectangle {
            id: delegate

            height: UIConstants.LIST_ITEM_HEIGHT_DEFAULT
            width: listView.width
            color: "transparent"

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 16

                    Image {
                        height: 64
                        width: 64
                        source: "image://theme/" + accountIcon
                    }

                    Column {
                        Label {
                            text: accountName
                            style: LabelStyleTitle {}
                            font.weight: Font.Bold
                        }
                        Label {
                            text: {
                                switch (presenceRule.action) {
                                case PresenceRule.SetOffline:
                                    return "Set to offline";
                                case PresenceRule.SetOnline:
                                    return "Set to online";
                                case PresenceRule.Retain:
                                    return "Do not change";
                                }
                            }
                            style: LabelStyleSubtitle {}
                        }
                    }
                }

                onClicked: {
                    presenceSelectionDialog.rule = presenceRule
                    presenceSelectionDialog.open();
                }
            }

            states: [
                State {
                    name: "pressed"
                    when: mouseArea.pressed
                    PropertyChanges {
                        target: delegate
                        color: UIConstants.COLOR_SELECT
                    }
                }
            ]
        }

        Label {
            anchors.centerIn: parent
            visible: listView.count == 0

            text: "No accounts found"
            style: LabelStyle {
                textColor: UIConstants.TITLE_INVERTED_COLOR
                fontPixelSize: UIConstants.FONT_XLARGE
            }
        }
    }

    RuleTopicSummary {
        id: restorePresences

        anchors { right: parent.right; bottom: parent.bottom; left: parent.left }
        height: visible ? implicitHeight : 0
        visible: {
            for (var row = 0; row < listView.model.presenceRules.length; row++) {
                if (listView.model.presenceRules[row].action != PresenceRule.Retain)
                    return true;
            }

            return false;
        }
        topic: "Restore previous availability"
        topicHeight: Math.max(topicImplicitHeight, restoreSwitch.height)
        summary: restoreSwitch.checked ? "Previous availability will be restored."
                                       : "Previous availability will not be restored."
        onTopicClicked: restoreSwitch.checked = !restoreSwitch.checked
        Switch {
            id: restoreSwitch
            checked: rule.restorePresence
            anchors { top: parent.top; right: parent.right; verticalCenter: parent.top }
            onCheckedChanged: rule.restorePresence = checked
        }

        Behavior on height { NumberAnimation { duration: 250 } }
    }

    SelectionDialog {
        id: presenceSelectionDialog

        property PresenceRule rule

        titleText: "Availability changed to"
        model: ListModel {
            ListElement { name: "Online"; value: PresenceRule.SetOnline }
            ListElement { name: "Offline"; value: PresenceRule.SetOffline }
            ListElement { name: "Do not change"; value: PresenceRule.Retain }
        }
        platformStyle: SelectionDialogStyle {
           itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
        }

        onSelectedIndexChanged: {
            rule.action = model.get(selectedIndex).value
        }

        onRuleChanged: {
            if (!rule)
                return;

            for (var row = 0; row < model.count; row++) {
                if (model.get(row).value !== rule.action)
                    continue;

                presenceSelectionDialog.selectedIndex = row
                break;
            }
        }
    }
}
