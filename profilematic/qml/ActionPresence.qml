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
    property RuleAction action;

    tools: commonTools

    anchors.leftMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin
    anchors.rightMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin

    PageHeader {
        id: header
        text: qsTr("Account availability action")
    }

    TextField {
        id: statusMessage

        anchors { top: header.bottom; left: parent.left; right: parent.right; topMargin: UIConstants.DEFAULT_MARGIN }
        height: visible ? statusMessage.implicitHeight : 0
        visible: {
            for (var row = 0; row < action.presenceRules.length; row++) {
                switch (action.presenceRules[row].action) {
                case PresenceRule.SetOffline:
                case PresenceRule.Retain:
                    continue;
                default:
                    return true;
                }
            }

            return false;
        }
        style: TextFieldStyle {
            paddingRight: clearButton.width
        }

        placeholderText: qsTr("Status message")
        text: action.presenceStatusMessage

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

        onTextChanged: action.presenceStatusMessage = text

        Keys.onReturnPressed: {
            platformCloseSoftwareInputPanel()
            dummy.focus = true
        }
    }

    Item { id: dummy }

    Row {
        id: buttonBar

        spacing: UIConstants.PADDING_SMALL
        anchors { top: statusMessage.bottom; right: parent.right; left: parent.left; topMargin: UIConstants.DEFAULT_MARGIN }
        visible: listView.count > 0

        Button {
            id: allOnlineButton

            property bool shouldBeChecked: root.action.presenceChangeType === RuleAction.AllOnlinePresenceType

            text: qsTr("All online")
            width: (parent.width - parent.spacing) / 2
            checkable: true
            checked: shouldBeChecked

            onClicked: root.action.presenceChangeType = (checked ? RuleAction.AllOnlinePresenceType :  RuleAction.CustomPresenceType)

            Connections {
                target: root.action
                onPresenceChangeTypeChanged: allOnlineButton.checked = allOnlineButton.shouldBeChecked
            }
        }

        Button {
            id: allOfflineButton

            property bool shouldBeChecked: root.action.presenceChangeType === RuleAction.AllOfflinePresenceType

            text: qsTr("All offline")
            width: (parent.width - parent.spacing) / 2
            checkable: true
            checked: shouldBeChecked

            onClicked: root.action.presenceChangeType = (checked ? RuleAction.AllOfflinePresenceType : RuleAction.CustomPresenceType)

            Connections {
                target: root.action
                onPresenceChangeTypeChanged: allOfflineButton.checked = allOfflineButton.shouldBeChecked
            }
        }
    }

    ListView {
        id: listView

        clip: true
        anchors { top: buttonBar.bottom; right: parent.right; bottom: restorePresencesContainer.top; left: parent.left; topMargin: UIConstants.DEFAULT_MARGIN }
        model: AccountsModel {
            action: root.action
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
                                    return qsTr("Set to offline")
                                case PresenceRule.SetOnline:
                                    return qsTr("Set to online")
                                case PresenceRule.SetAway:
                                    return qsTr("Set to away")
                                case PresenceRule.SetBusy:
                                    return qsTr("Set to busy")
                                case PresenceRule.SetHidden:
                                    return qsTr("Set to invisible");
                                case PresenceRule.SetXa:
                                    return qsTr("Set to extended away");
                                case PresenceRule.Retain:
                                    return qsTr("Do not change");
                                }
                            }
                            style: LabelStyleSubtitle {}
                        }
                    }
                }

                onClicked: {
                    console.log("onClicked presenceRule", presenceRule)
                    presenceSelectionDialog.supportedPresences = supportedPresences
                    presenceSelectionDialog.rule = presenceRule
                    presenceSelectionDialog.open()

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

            text: qsTr("No accounts found")
            style: LabelStyle {
                textColor: UIConstants.TITLE_INVERTED_COLOR
                fontPixelSize: UIConstants.FONT_XLARGE
            }
        }
    }

    Item {
        id: restorePresencesContainer
        anchors { right: parent.right; bottom: parent.bottom; left: parent.left }
        height: isVisible() ? restorePresences.height : 0
        clip: true

        function isVisible() {
            for (var row = 0; row < action.presenceRules.length; row++) {
                if (action.presenceRules[row].action !== PresenceRule.Retain)
                    return true;
            }

            return false;
        }

        Behavior on height { NumberAnimation { duration: 250 } }

        RuleTopicSummary {
            id: restorePresences

            topic: qsTr("Restore previous availability")
            topicHeight: Math.max(topicImplicitHeight, restoreSwitch.height)
            topicWidth: parent.width - restoreSwitch.width
            summary: restoreSwitch.checked ? qsTr("Previous availability will be restored.")
                                           : qsTr("Previous availability will not be restored.")
            onTopicClicked: restoreSwitch.checked = !restoreSwitch.checked
        }

        Switch {
            id: restoreSwitch
            checked: action.restorePresence
            anchors { top: parent.top; right: parent.right; verticalCenter: parent.top }
            onCheckedChanged: action.restorePresence = checked
        }
    }

    SelectionDialog {
        id: presenceSelectionDialog

        property PresenceRule rule
        property variant supportedPresences
        property variant __presenceModelList: ListModel {
             id: presenceModel

            function addElement(name, value) {
                append({
                           "name"  : name,
                           "value" : value
                       })
            }

            function initialize() {
                // ListElement can't have JavaScript (ie. qsTr), so have to create the model like this
                if (count === 0) {
                    addElement(qsTr("Online"),        PresenceRule.SetOnline)
                    addElement(qsTr("Offline"),       PresenceRule.SetOffline )
                    addElement(qsTr("Away"),          PresenceRule.SetAway)
                    addElement(qsTr("Extended away"), PresenceRule.SetXa )
                    addElement(qsTr("Busy"),          PresenceRule.SetBusy )
                    addElement(qsTr("Invisible"),     PresenceRule.SetHidden)
                }
            }

            Component.onCompleted: {
                initialize()
            }

        }

        function __updateSelectedIndex() {
            presenceSelectionDialog.selectedIndex = -1
            console.log("__updateSelectedIndex, rule", rule)
            if (!rule)
                return;

            console.log("__updateSelectedIndex, model.count", model.count)

            for (var row = 0; row < model.count; row++) {
                console.log("Row " + row + " value " + model.get(row).value + " rule.action " + rule.action)
                if (model.get(row).value !== rule.action)
                    continue;

                presenceSelectionDialog.selectedIndex = row
                break;
            }
        }

        model: ListModel {}
        titleText: qsTr("Availability changed to")
        platformStyle: SelectionDialogStyle {
           itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
        }

        onSelectedIndexChanged: {
            if (selectedIndex >= 0)
                rule.action = model.get(selectedIndex).value
        }

        onSupportedPresencesChanged: {
            // Create a new model rather than clearing the existing one:
            // There are some redraw issues if the current model is cleared
            // and filled with new values.
            presenceModel.initialize()

            var model = Qt.createQmlObject("import QtQuick 1.1; ListModel {}", presenceSelectionDialog)
            for (var i = 0; i < __presenceModelList.count; i++) {
                for (var j = 0; j < supportedPresences.length; j++) {
                    var entry = __presenceModelList.get(i)
                    if (entry.value === supportedPresences[j])
                        model.append(entry)
                }
            }

            model.append({ name: qsTr("Do not change"), value: PresenceRule.Retain })

            presenceSelectionDialog.selectedIndex = -1
            presenceSelectionDialog.model = model
        }

        onRuleChanged: presenceSelectionDialog.__updateSelectedIndex()

        Connections {
            target: presenceSelectionDialog.rule
            onActionChanged: presenceSelectionDialog.__updateSelectedIndex()
        }
    }
}
