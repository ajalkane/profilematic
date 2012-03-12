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
import profilematic 1.0

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

    function saveRule() {
        editRuleMenu.close();
        saved()
        pageStack.pop()
    }

    function ruleNeedsWarning() {
        var warnings = []
        if (rule.flightMode === 0 && (rule.wlan.length > 0 || rule.locationCells.length > 0)){
            warnings.push('Your condition on turning flight mode off is based on WLAN connection or Cell location. If flight mode is on, WLAN and Cell location are not usually usable.')
        }
        if (rule.powerSavingMode === 0 && (rule.wlan.length > 0 || rule.locationCells.length > 0)){
            warnings.push('Your condition on turning power saving mode off is based on WLAN connection. If flight mode is on, WLAN connection is not usually active.')
        }

        if (warnings.length > 0) {
            var warning = "<ul>\n"
            // warnings.join("<li>")
            for (var i = 0; i < warnings.length; ++i) {
                warning += "<li>" + warnings[i] + "</li>\n"
            }
            warning += "</ul>\n"
            console.log("Warning message", warning)
            dRuleWarning.message = warning
            dRuleWarning.open()
            return true
        }
        return false
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

    QueryDialog {
        id: dRuleWarning

        titleText: "Warning: this rule might not make sense"
        acceptButtonText: "Save anyway"
        rejectButtonText: "Cancel"

        onAccepted: {
            saveRule()
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
                    if (!ruleNeedsWarning()) {
                        saveRule()
                    }
                } else {
                    dInvalidRule.open()
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
                topic: "Cell id location"
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
                topic: "Profile"
                summary: profileSummary();
                showDrillDown: true
                onTopicClicked: profileEditHandler()                
            }

            RuleTopicSummary {
                id: presenceAction
                topic: "Account availability"
                summary: presenceSummary()
                showDrillDown: true
                onTopicClicked: presenceEditHandler()
            }

            RuleTopicSummary {
                topic: "Flight mode"
                summary: flightModeSummary();
                showDrillDown: true
                onTopicClicked: flightModeEditHandler()
                visible: true
            }

            RuleTopicSummary {
                topic: "Power saving mode"
                summary: powerSavingModeSummary();
                showDrillDown: true
                onTopicClicked: powerSavingModeEditHandler()
                visible: true
            }

            RuleTopicSummary {
                id: blueToothAction
                topic: "BlueTooth"
                summary: blueToothModeSummary();
                showDrillDown: true
                onTopicClicked: blueToothModeEditHandler()
            }

            RuleTopicSummary {
                id: cellularAction
                topic: "Mobile network mode"
                summary: cellularModeSummary();
                showComboBox: true
                onTopicClicked: cellularModeEditHandler()
            }

            RuleTopicSummary {
                id: commandLineAction
                topic: "Custom action"
                summary: commandLineSummary();
                showDrillDown: true
                onTopicClicked: commandLineEditHandler()
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
                    onPresenceRulesChanged: ruleSummary.text = root.ruleSummary()
                    onFlightModeChanged:    ruleSummary.text = root.ruleSummary()
                    onPowerSavingModeChanged: ruleSummary.text = root.ruleSummary()
                    onBlueToothModeChanged: ruleSummary.text = root.ruleSummary()
                    onCellularModeChanged:  ruleSummary.text = root.ruleSummary()
                    onStandByScreenModeChanged: ruleSummary.text = root.ruleSummary()
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

    // Flight mode functions
    ActionFlightMode {
        id: actionFlightMode
        rule: root.rule
    }

    function flightModeSummary() {
        if (rule.flightMode >= 0) {
            var summary = actionFlightMode.flightModeSummary()

            if (rule.restoreFlightMode) {
                summary += ". Restores previous mode."
            }
            return summary
        }
        return "Click to set"
    }

    function flightModeEditHandler() {
        pageStack.push(actionFlightMode)
    }

    // Power Saving mode functions
    ActionPowerSavingMode {
        id: actionPowerSavingMode
        rule: root.rule
    }

    function powerSavingModeSummary() {
        if (rule.powerSavingMode >= 0) {
            var summary = actionPowerSavingMode.powerSavingModeSummary()

            if (rule.restorePowerSavingMode) {
                summary += ". Restores previous mode."
            }
            return summary
        }
        return "Click to set"
    }


    function powerSavingModeEditHandler() {
        pageStack.push(actionPowerSavingMode)
    }

    // BlueTooth mode
    ActionBlueToothMode {
        id: actionBlueToothMode
        rule: root.rule
    }

    function blueToothModeSummary() {
        if (rule.blueToothMode >= 0) {
            var summary = actionBlueToothMode.blueToothModeSummary()

            if (rule.restoreBlueToothMode) {
                summary += ". Restores previous mode."
            }
            return summary
        }
        return "Click to set"
    }

    function blueToothModeEditHandler() {
        pageStack.push(actionBlueToothMode)
    }

    // Cellular mode
    CellularModeDialog {
        id: dCellularMode

        onCellularModeSelected: {
            rule.cellularMode = selectedCellularMode
        }
    }

    function cellularModeSummary() {
        return dCellularMode.cellularModeToText(rule.cellularMode)
    }

    function cellularModeEditHandler() {
        dCellularMode.selectedCellularMode = rule.cellularMode
        dCellularMode.open();
    }

    // CommandLine
    ActionCommandLine {
        id: actionCommandLine
        rule: root.rule
    }

    function commandLineSummary() {
        if (rule.commandLine !== "") {
            return "Custom action has been set"
        }
        return "Click to set"
    }

    function commandLineEditHandler() {
        pageStack.push(actionCommandLine)
    }

    // Presence
    function presenceSummary() {
        var summary = "";
        var atLeastOneChange = false;

        for (var row = 0; row < rule.presenceRules.length; row++) {
            if (rule.presenceRules[row].action !== PresenceRule.Retain) {
                atLeastOneChange = true;
                break;
            }
        }

        if (atLeastOneChange)
            summary = "At least one change";
        else
            summary = "Don't change"

        if (rule.restorePresence)
            summary += ". Restores previous availability";

        return summary;
    }

    function presenceEditHandler() {
        root.pageStack.push(Qt.resolvedUrl("ActionPresence.qml"), { 'rule': rule });
    }

}
