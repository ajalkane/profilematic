/*********************************************************************
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
        if (rule.action.flightMode === 0 && (rule.wlan.length > 0 || rule.locationCells.length > 0)){
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
                    target: rule.condition
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
                    target: rule.condition
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
                    target: rule.condition
                    onWlanChanged: wlanCondition.summary = wlanSummary()
                    onWlanTimeoutChanged: wlanCondition.summary = wlanSummary()
                }
            }

            RuleTopicSummary {
                id: idleCondition
                topic: "Idle"
                summary: idleSummary()
                showDrillDown: true
                onTopicClicked: idleEditHandler()
                visible: !rule.isDefaultRule
                Connections {
                    target: rule.condition
                    onIdleForSecsChanged: idleCondition.summary = idleSummary()
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

//            RuleTopicSummary {
//                topic: "Flight mode"
//                summary: flightModeSummary();
//                showDrillDown: true
//                onTopicClicked: flightModeEditHandler()
//            }

//            RuleTopicSummary {
//                topic: "Power saving mode"
//                summary: powerSavingModeSummary();
//                showDrillDown: true
//                onTopicClicked: powerSavingModeEditHandler()
//            }

//            RuleTopicSummary {
//                id: blueToothAction
//                topic: "BlueTooth"
//                summary: blueToothModeSummary();
//                showDrillDown: true
//                onTopicClicked: blueToothModeEditHandler()
//            }

//            RuleTopicSummary {
//                id: cellularAction
//                topic: "Mobile network mode"
//                summary: cellularModeSummary();
//                showComboBox: true
//                onTopicClicked: cellularModeEditHandler()
//            }

//            RuleTopicSummary {
//                topic: "Stand-by screen mode"
//                summary: standByScreenModeSummary();
//                showDrillDown: true
//                onTopicClicked: standByScreenModeEditHandler()
//            }

//            RuleTopicSummary {
//                topic: "Background connections"
//                summary: backgroundConnectionsModeSummary();
//                showDrillDown: true
//                onTopicClicked: backgroundConnectionsModeEditHandler()
//            }

//            RuleTopicSummary {
//                id: commandLineAction
//                topic: "Custom action"
//                summary: commandLineSummary();
//                showDrillDown: true
//                onTopicClicked: commandLineEditHandler()
//            }

            Text {
                id: ruleSummary
                wrapMode: Text.WordWrap
                width: parent.width
                font.pixelSize: UIConstants.FONT_SMALL;
                color: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                text: root.ruleSummary()
                Connections {
                    target: rule
                    onChanged: ruleSummary.text = root.ruleSummary();
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
        action: root.rule.action
    }

    function profileSummary() {
        if (rule.action.profile !== "") {
            var profile = backendProfilesModel.getProfileToName(rule.action.profile)
            var summary = profile

            if (backendProfilesModel.profileHasVolume(rule.action.profile) && rule.action.profileVolume > -1) {
                summary += " (" + rule.action.profileVolume + "%)"
            }
            if (rule.action.restoreProfile) {
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
        condition: root.rule.condition
    }

    function timeSummary() {
        return backendRulesModel.getTimeSummaryText(rule.condition, "Not in use");
    }

    function timeEditHandler() {
        pageStack.push(conditionTime)
    }

    // Location functions
    ConditionLocation {
        id: conditionLocation
        condition: root.rule.condition
    }

    function locationSummary() {
        var numCellIds = rule.condition.locationCells.length
        return numCellIds === 0 ? "Not in use" : "Cell ids set"
    }

    function locationEditHandler() {
        pageStack.push(conditionLocation)
    }

    // Wlan functions
    ConditionWlan {
        id: conditionWlan
        condition: root.rule.condition
    }

    function wlanSummary() {
        var numWlans = rule.condition.wlan.length
        return numWlans === 0 ? "Not in use"
                              : "In use"
                                + (rule.condition.wlanTimeout > 0 ? " (" + rule.condition.wlanTimeout + "s timeout)"
                                                        : "")
    }

    function wlanEditHandler() {
        pageStack.push(conditionWlan)
    }

    // Idle functions
    ConditionIdle {
        id: conditionIdle
        condition: root.rule.condition
    }

    function idleSummary() {
        console.log("idleSummary", rule.condition.idleForSecs)
        return rule.condition.idleForSecs < 0 ? "Not in use"
                              : "At least for " + rule.condition.idleForSecs + " seconds"
    }

    function idleEditHandler() {
        pageStack.push(conditionIdle)
    }

    // Flight mode functions
    ActionFlightMode {
        id: actionFlightMode
        action: root.rule.action
    }

    function flightModeSummary() {
        if (rule.action.flightMode >= 0) {
            var summary = actionFlightMode.flightModeSummary()

            if (rule.action.restoreFlightMode) {
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
        action: root.rule.action
    }

    function powerSavingModeSummary() {
        var summary = ""
        if (rule.action.powerSavingMode >= 0) {
            summary = actionPowerSavingMode.powerSavingModeSummary()

            if (rule.action.restorePowerSavingMode) {
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
        action: root.rule.action
    }

    function blueToothModeSummary() {
        if (rule.action.blueToothMode >= 0) {
            var summary = actionBlueToothMode.blueToothModeSummary()

            if (rule.action.restoreBlueToothMode) {
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
            rule.action.cellularMode = selectedCellularMode
        }
    }

    function cellularModeSummary() {
        return dCellularMode.cellularModeToText(rule.action.cellularMode)
    }

    function cellularModeEditHandler() {
        dCellularMode.selectedCellularMode = rule.action.cellularMode
        dCellularMode.open();
    }

    // Stand-by screen
    ActionStandByScreen {
        id: actionStandByScreenMode
        action: root.rule.action
    }

    function standByScreenModeSummary() {
        var summary = ""
        if (rule.action.standByScreenMode >= 0) {
            summary = actionStandByScreenMode.standByScreenModeSummary()

            if (rule.action.restoreStandByScreenMode) {
                summary += ". Restores previous mode."
            }
            return summary
        }
        return "Click to set"
    }


    function standByScreenModeEditHandler() {
        pageStack.push(actionStandByScreenMode)
    }

    // Background connections
    ActionBackgroundConnections {
        id: actionBackgroundConnectionsMode
        action: root.rule.action
    }

    function backgroundConnectionsModeSummary() {
        var summary = ""
        if (rule.action.backgroundConnectionsMode >= 0) {
            summary = actionBackgroundConnectionsMode.backgroundConnectionsModeSummary()

            if (rule.action.restoreBackgroundConnectionsMode) {
                summary += ". Restores previous mode."
            }
            return summary
        }
        return "Click to set"
    }


    function backgroundConnectionsModeEditHandler() {
        pageStack.push(actionBackgroundConnectionsMode)
    }

    // CommandLine
    ActionCommandLine {
        id: actionCommandLine
        action: root.rule.action
    }

    function commandLineSummary() {
        if (rule.action.commandLine !== "") {
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

        for (var row = 0; row < rule.action.presenceRules.length; row++) {
            if (rule.action.presenceRules[row].action !== PresenceRule.Retain) {
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
        root.pageStack.push(Qt.resolvedUrl("ActionPresence.qml"), { 'action': rule.action });
    }

}
