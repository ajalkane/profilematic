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
        if (rule.action.flightMode === 0 && (rule.condition.wlan.length > 0 || rule.condition.locationCells.length > 0)){
            warnings.push('Your condition on turning flight mode off is based on WLAN connection or Cell location. If flight mode is on, WLAN and Cell location are not usually usable.')
        }
        if (rule.action.powerSavingMode === 0 && (rule.condition.wlan.length > 0 || rule.condition.locationCells.length > 0)){
            warnings.push('Your condition on turning power saving mode off is based on WLAN connection. If power saving mode is on, WLAN connection is not usually active.')
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
            MenuItem {
                text: rule.ruleActive ? "Deactivate rule" : "Activate rule"
                enabled: !rule.isDefaultRule
                onClicked: {
                    rule.ruleActive = !rule.ruleActive
                }
            }
        }
    }

    ItemSelectionDialog {
        id: dMoreConditions
        titleText: "Select condition"
        model: backendConditionEditNonVisibleModel
        onItemSelected: {
            console.log("on item selected called ", item.qmlEditFile)
            conditionEditHandler(item.qmlEditFile)
        }
    }

    ItemSelectionDialog {
        id: dMoreActions
        titleText: "Select action"
        model: backendActionEditNonVisibleModel
        onItemSelected: {
            console.log("on item selected called ", item.qmlEditFile)
            actionEditHandler(item.qmlEditFile)
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
            LabelHelp {
                visible: rule.isDefaultRule
                text: "Activates specified actions if no other rule with matching conditions has the action specified. "
                      + "Note: if a regular rule has 'restore previous' in its action, it overrides default rules action."
            }
            LabelHelp {
                visible: !rule.ruleActive
                color: "orange" // IMPROVE to UIConstants
                text: "This rule is not active. You can reactivate this rule from menu. Changes won't be activated before saving."
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

            Repeater {
                id: conditionsView
                model: backendConditionEditVisibleModel
                Item {
                    width: root.width
                    height: childrenRect.height
                    visible: !rule.isDefaultRule

                    RuleTopicSummary {
                        topic: model.name
                        summary: model.summary
                        showDrillDown: true
                        onTopicClicked: {
                            conditionEditHandler(model.qmlEditFile);
                            console.log("Clicked qmlEditHandler " + qmlEditFile);
                        }
                        onPressAndHold: {
                            contextMenuConditions.openForIndex(index, model.name)
                        }

                        // visible: model.visible
                    }
                }
            }

            Button {
                text: "More conditions"
                visible: !rule.isDefaultRule && backendConditionEditNonVisibleModel.count > 0
                onClicked: {
                    console.log("More conditions clicked")
                    console.log("Size: " + backendConditionEditNonVisibleModel.count)
                    dMoreConditions.selectedIndex = -1
                    dMoreConditions.open()
                }
            }


            SectionHeader {
                section: "Action"
            }

            Repeater {
                id: actionsView
                model: backendActionEditVisibleModel

                Item {
                    width: root.width
                    height: childrenRect.height

                    RuleTopicSummary {
                        topic: model.name
                        summary: model.summary
                        showDrillDown: true
                        onTopicClicked: {
                            actionEditHandler(model.qmlEditFile);
                            console.log("Clicked qmlEditHandler " + qmlEditFile);
                        }

                        // visible: model.visible
                    }
                }
            }

            Button {
                text: "More actions"
                visible: backendActionEditNonVisibleModel.count > 0
                onClicked: {
                    console.log("More actions clicked")
                    console.log("Size: " + backendActionEditNonVisibleModel.count)
                    dMoreActions.selectedIndex = -1
                    dMoreActions.open()
                }
            }


            LabelHelp {
                id: ruleSummary
                text: root.ruleSummary()
                Connections {
                    target: rule
                    onChanged: ruleSummary.text = root.ruleSummary();
                }
            }
        } // Column
    } // Flickable

    ContextMenu {
        id: contextMenuConditions
        property int selectedIndex
        property string topic;

        MenuLayout {
            MenuItem {
                text: "Clear " + contextMenuConditions.topic
                onClicked: {
                    console.log("Clear condition index ", contextMenuConditions.selectedIndex)
                    // backendEdRulesModel.toggleRuleActive(contextMenu.selectedRuleIndex)
                }
            }
        }
        function openForIndex(index, topic) {
            selectedIndex = index
            contextMenuConditions.topic = topic
            open()
        }
    }

    function conditionEditHandler(fileName) {
        root.pageStack.push(Qt.resolvedUrl(fileName), { 'condition': rule.condition });
    }

    function actionEditHandler(fileName) {
        root.pageStack.push(Qt.resolvedUrl(fileName), { 'action': rule.action });
    }

    function ruleSummary() {
        return backendRulesModel.getRuleSummaryText(rule, "Can't be used as a rule yet. Specify at least one condition, and an action.");
    }
}
