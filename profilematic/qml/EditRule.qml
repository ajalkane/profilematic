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
    id: root
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
        var emptyStringIfInvalid = backendRulesModel.getRuleSummaryText(rule, "")
        return emptyStringIfInvalid !== ""
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
                // height: UIConstants.LIST_ITEM_HEIGHT_SMALL
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
                id: timeCondition
                topic: "Time"
                summary: timeSummary()
                showDrillDown: true
                onTopicClicked: timeEditHandler()
                visible: !rule.isDefaultRule
                Connections {
                    target: rule
                    onDaysChanged:      timeCondition.summary = timeSummary()
                    onTimeStartChanged: timeCondition.summary = timeSummary()
                    onTimeEndChanged:   timeCondition.summary = timeSummary()
                }
            }

            RuleTopicSummary {
                id: locationCondition
                topic: "Location"
                summary: locationSummary()
                showDrillDown: true
                onTopicClicked: locationEditHandler()
                visible: !rule.isDefaultRule
                Connections {
                    target: rule
                    onLocationCellsChanged: locationCondition.summary = locationSummary()
                }
            }

            RuleTopicSummary {
                id: wlanCondition
                topic: "WLAN"
                summary: wlanSummary()
                showDrillDown: true
                onTopicClicked: wlanEditHandler()
                visible: !rule.isDefaultRule
                Connections {
                    target: rule
                    onWlanChanged: wlanCondition.summary = wlanSummary()
                    onWlanTimeoutChanged: wlanCondition.summary = wlanSummary()
                }

            }

            SectionHeader {
                section: "Action"
            }

            RuleTopicSummary {
                topic: "Set profile"
                summary: profileSummary();
                showDrillDown: true
                onTopicClicked: profileEditHandler()                
            }

            RuleTopicSummary {
                topic: "Set flight mode"
                summary: flightModeSummary();
                showComboBox: true
                onTopicClicked: flightModeEditHandler()
                // Flight mode needs open kernel. Needs a bit more work until proper support can
                // be given.
                visible: false
            }

            RuleTopicSummary {
                id: blueToothAction
                topic: "Set BlueTooth"
                summary: blueToothModeSummary();
                showComboBox: true
                onTopicClicked: blueToothModeEditHandler()
            }

            Text {
                id: ruleSummary
                wrapMode: Text.WordWrap
                width: parent.width
                font.pixelSize: UIConstants.FONT_SMALL;
                color: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                text: root.ruleSummary()
                Connections {
                    target: rule
                    onDaysChanged:          ruleSummary.text = root.ruleSummary()
                    onTimeStartChanged:     ruleSummary.text = root.ruleSummary()
                    onTimeEndChanged:       ruleSummary.text = root.ruleSummary()
                    onLocationCellsChanged: ruleSummary.text = root.ruleSummary()
                    onWlanChanged:          ruleSummary.text = root.ruleSummary()
                    onProfileChanged:       ruleSummary.text = root.ruleSummary()
                    onBlueToothModeChanged: ruleSummary.text = root.ruleSummary()
                }
            }
        } // Column
    } // Flickable

    function ruleSummary() {
        return backendRulesModel.getRuleSummaryText(rule, "Can't be used as a rule yet. Specify at least one condition, and an action.");
    }

    // Profile functions
    ActionProfile {
        id: actionProfile
        rule: root.rule
    }

    function profileSummary() {
        if (rule.profile !== "") {
            var profile = backendProfilesModel.getProfileToName(rule.profile)
            var summary = profile

            if (backendProfilesModel.profileHasVolume(rule.profile) && rule.profileVolume > -1) {
                summary += " (" + rule.profileVolume + "%)"
            }
            if (rule.restoreProfile) {
                summary += ". Restores previous profile."
            }
            return summary
        }
        return "Click to set"
    }

    function profileEditHandler() {
        pageStack.push(actionProfile)
    }

    // Time functions
    ConditionTime {
        id: conditionTime
        rule: root.rule
    }

    function timeSummary() {
        return backendRulesModel.getTimeSummaryText(rule, "Not in use");
    }

    function timeEditHandler() {
        pageStack.push(conditionTime)
    }

    // Location functions
    ConditionLocation {
        id: conditionLocation
        rule: root.rule
    }

    function locationSummary() {        
        var numCellIds = rule.locationCells.length
        return numCellIds === 0 ? "Not in use" : "Cell ids set"
    }

    function locationEditHandler() {
        pageStack.push(conditionLocation)
    }

    // Wlan functions
    ConditionWlan {
        id: conditionWlan
        rule: root.rule
    }

    function wlanSummary() {
        var numWlans = rule.wlan.length
        return numWlans === 0 ? "Not in use"
                              : "In use"
                                + (rule.wlanTimeout > 0 ? " (" + rule.wlanTimeout + "s timeout)"
                                                        : "")
    }

    function wlanEditHandler() {
        pageStack.push(conditionWlan)
    }

    // Flight mode
    FlightModeDialog {
        id: dFlightMode

        onFlightModeSelected: {
            rule.flightMode = selectedFlightMode
        }
    }

    function flightModeSummary() {
        var flightModeText = dFlightMode.flightModeToText(rule.flightMode)
        return flightModeText
    }

    function flightModeEditHandler() {
        dFlightMode.selectedFlightMode = rule.flightMode
        dFlightMode.open();
    }

    // BlueTooth mode
    BlueToothModeDialog {
        id: dBlueToothMode

        onBlueToothModeSelected: {
            rule.blueToothMode = selectedBlueToothMode
        }
    }

    function blueToothModeSummary() {
        return dBlueToothMode.blueToothModeToText(rule.blueToothMode)
    }

    function blueToothModeEditHandler() {
        dBlueToothMode.selectedBlueToothMode = rule.blueToothMode
        dBlueToothMode.open();
    }

}
