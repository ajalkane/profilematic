/**********************************************************************
 * Copyright 2011 Arto Jalkanen
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
import com.nokia.extras 1.0

import Rule 1.0

import "UIConstants.js" as UIConstants

Page {    
    id: editRule
    tools: editRuleTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property Rule    rule;

    signal saved
    signal cancelled
    signal deleted

//    onStatusChanged: {
//        console.log("Status changed ", status)
//        if (status == 1) {
//            // volumeVisibility();
//        }
//    }

    function isValidRule() {
        if (rule.isDefaultRule) {
            return true;
        }

        var isConditionsValid = false
        var isActionsValid = false

        var isTimeValid = false
        if (rule.days.length !== 0 && rule.timeStart !== "" && rule.timeEnd !== "") {
            isTimeValid = true
        }

        if (isTimeValid) {
            isConditionsValid = true
        }

        if (rule.profile !== "") {
            isActionsValid = true
        }
        console.log("conditionsValid, actionsValid", isConditionsValid, isActionsValid)
        return isConditionsValid && isActionsValid
    }

    function openFile(file) {
        var component = Qt.createComponent(file)

        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    function confirmDelete() {
        dConfirmDelete.open()
    }

    QueryDialog {
        id: dInvalidRule

        titleText: "This rule can not be used"
        message: "A rule must have at least one condition, and one action selected"
        acceptButtonText: "Ok"
    }

    QueryDialog {
        id: dConfirmDelete

        titleText: "Delete this rule?"
        acceptButtonText: "Delete"
        rejectButtonText: "Cancel"

        onAccepted: {
            deleted()
            pageStack.pop()
        }
    }

    ToolBarLayout {
        id: editRuleTools

        ToolButton {
            anchors.left: parent.left
            anchors.leftMargin: UIConstants.PADDING_XLARGE
            text: "Save"
            onClicked: {
                if (isValidRule()) {
                    editRuleMenu.close();
                    saved()
                    pageStack.pop()
                } else {
                    dInvalidRule.open();
                }
            }
        }
        ToolButton {
            text: "Cancel"
            onClicked: {
                editRuleMenu.close();
                cancelled()
                pageStack.pop()
            }
        }
        ToolIcon {
            visible: rule.ruleId !== '' //  -1
            iconId: "toolbar-view-menu";
            anchors.right: parent.right
            onClicked: (editRuleMenu.status == DialogStatus.Closed) ? editRuleMenu.open() : editRuleMenu.close()
        }
    }

    Menu {
        id: editRuleMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: "Delete"
                onClicked: {
                    console.log("Delete menu item clicked")
                    confirmDelete()
                }
            }
        }
    }

    Flickable {
        anchors.fill: parent // editRule
        pressDelay: 140
        clip: true
        contentWidth: parent.width
        contentHeight: container.height // contentItem.childrenRect.height

        Column {
            id: container
            spacing: UIConstants.DEFAULT_MARGIN
            // anchors.fill: parent
            anchors.verticalCenter: parent.verticalCenter

            width: parent.width
            height: childrenRect.height

            Label {
                id: label
                color: UIConstants.COLOR_SECONDARY_FOREGROUND
                text: rule.ruleName
                visible: rule.isDefaultRule
            }

            TextFieldWithLabel {
                labelText: "Rule name"
                placeholderText: "Auto-generated"
                text: rule.ruleName
                height: UIConstants.LIST_ITEM_HEIGHT_SMALL
                width: parent.width
                visible: !rule.isDefaultRule
                onTextChanged: {
                    rule.ruleName = text
                }
            }

            SectionHeader {
                section: "Condition"
                visible: !rule.isDefaultRule
            }

            RuleTopicSummary {
                id: timeRule
                topic: "Time"
                summary: timeSummary()
                showDrillDown: true
                onTopicClicked: timeEditHandler()
                visible: !rule.isDefaultRule
                Connections {
                    target: rule
                    onDaysChanged:      timeRule.summary = timeSummary()
                    onTimeStartChanged: timeRule.summary = timeSummary()
                    onTimeEndChanged:   timeRule.summary = timeSummary()
                }

            }

            SectionHeader {
                section: "Action"
            }

            RuleTopicSummary {
                topic: "Set profile"
                summary: profileSummary();
                onTopicClicked: profileEditHandler()
            }

            Item {
                width: parent.width
                height: volume.visible ? volume.height : 0
                clip: true

                Behavior on height {
                    NumberAnimation { duration: 300 }
                }

                RuleTopicSummary {
                    id: volume
                    topic: "Set volume"
                    summary: volumeSummary();
                    visible: isVolumeVisible();
                    onTopicClicked: volumeEditHandler()
                }
            }

            RuleTopicSummary {
                topic: "Set flight mode"
                summary: flightModeSummary();
                onTopicClicked: flightModeEditHandler()
                // Flight mode needs open kernel. Needs a bit more work until proper support can
                // be given.
                visible: false
            }

            Text {
                wrapMode: Text.WordWrap
                width: parent.width
                // visible: isValidRule()
                font.pixelSize: UIConstants.FONT_SMALL;
                color: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                text: {
                    if (isValidRule()) {
                        var help = "This rule activates profile " + backendProfilesModel.getProfileToName(rule.profile)
                        if (rule.isDefaultRule) {
                            help += " when other rules don't apply";
                        } else {
                            help += " when clock is "
                                    + (rule.timeStart === rule.timeEnd
                                       ? rule.timeStart
                                       : "between " + rule.timeStart + " - " + rule.timeEnd)
                            help += " on active days"
                        }
                        return help
                    }
                    return ""
                }
            }
        } // Column
    } // Flickable

    // Time functions
    ConditionTime {
        id: conditionTime
        rule: editRule.rule
    }

    // Profile functions
    function timeSummary() {
        console.log("timeSummary")
        return backendRulesModel.getTimeSummaryText(rule, "Not in use");
    }

    function timeEditHandler() {
        pageStack.push(conditionTime)
    }

    MySelectionDialog {
        id: profilesDialog
        titleText: "Set profile"
        platformStyle: SelectionDialogStyle {
            itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
        }
        model: backendProfilesModel

        onSelectedIndexChanged: {
            if (selectedIndex > -1) {
                var selectedProfile = model.getProfile(selectedIndex)
                rule.profile = selectedProfile
            }
        }


        function openWithSelection(selectedProfile) {
            for (var i = 0; i < backendProfilesModel.count; i++) {
                var profile = backendProfilesModel.getProfile(i)
                if (selectedProfile == profile) {
                    selectedIndex = i
                }
            }
            open()
        }

    }

    // Profile functions
    function profileSummary() {
        return rule.profile !== "" ? backendProfilesModel.getProfileToName(rule.profile) : "Click to set"
    }

    function profileEditHandler() {
        profilesDialog.openWithSelection(rule.profile)
    }

    QueryDialog {
        id: dVolume

        titleText: "Choose ringing volume"
        acceptButtonText: "OK"
        rejectButtonText: "Cancel"
        property alias volumeValue: volumeSlider.value

        content: Item {
            Slider {
                id: volumeSlider
                // IMPROVE it might be cleaner to ask these from backend instead of hard-coding here
                minimumValue: 40;
                maximumValue: 100;
                stepSize: 20
                valueIndicatorVisible: false
                width: dVolume.width
            }
        }

        onAccepted: {
            rule.profileVolume = volumeValue
        }

        function openWithValue(volume) {
            volumeValue = volume
            open()
        }
    }

    // Profile volume functions
    function volumeSummary() {
        console.log("VolumeSummary called", rule.profileVolume)
        if (rule.profileVolume < 0) {
            return "Volume has not been selected yet"
        }

        return rule.profileVolume + " %"
    }

    function volumeEditHandler() {
        console.log("volumeEditHandler")
        dVolume.openWithValue(rule.profileVolume)
    }

    function isVolumeVisible() {
        return backendProfilesModel.profileHasVolume(rule.profile);
    }


    // Flight mode
    FlightModeDialog {
        id: dFlightMode

        onFlightModeSelected: {
            rule.flightMode = selectedFlightMode
        }
    }

    function flightModeSummary() {
        console.debug("flightModeSummary")
        var flightModeText = dFlightMode.flightModeToText(rule.flightMode)
        console.debug("flightModeSummary as text", flightModeText)
        return flightModeText
    }

    function flightModeEditHandler() {
        dFlightMode.selectedFlightMode = rule.flightMode
        dFlightMode.open();
    }
}
