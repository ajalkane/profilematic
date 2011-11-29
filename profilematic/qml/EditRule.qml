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

// IMPROVE: this is too complex for it's own good.
// Probably would work better without trying to force into ListView.
Page {
    id: editRule
    tools: editRuleTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property Rule    rule;

    signal saved
    signal cancelled
    signal deleted

    onStatusChanged: {
        // console.log("Status changed ", status)
        if (status == 1) {
            volumeVisibility();
        }
    }

    function isValidRule() {
        if (rule.days.length === 0 || rule.timeStart === "" || rule.profile === "") {
            return false;
        }
        return true;
    }

    function openFile(file) {
        var component = Qt.createComponent(file)

        if (component.status == Component.Ready)
            pageStack.push(component);
        else
            console.log("Error loading component:", component.errorString());
    }

    ToolBarLayout {
        id: editRuleTools

        ToolButton {
            anchors.left: parent.left
            anchors.leftMargin: UIConstants.PADDING_XLARGE
            text: "Accept"
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

    function formatTime(hour, minute) {
        return (hour < 10 ? "0" : "") + hour + ":" + (minute < 10 ? "0" : "") + minute
    }

    TimePickerDialog {
        id: timeStartDialog
        fields: DateTime.Hours | DateTime.Minutes
        hourMode: DateTime.TwentyFourHours
        titleText: "Time"
        acceptButtonText: "Ok"
        rejectButtonText: "Cancel"

        onAccepted: rule.timeStart = formatTime(hour, minute)
        property string targetProperty
    }

    // Start time functions
    function timeStartEditHandler() {
        var time = (rule.timeStart !== "" ? rule.timeStart : "00:00")
        var timeSplits = time.split(":")

        console.log("starTimeEditHandler timeSplits", timeSplits[0], timeSplits[1])

        timeStartDialog.hour = timeSplits[0]
        timeStartDialog.minute = timeSplits[1]

        timeStartDialog.open();
    }

    function timeStartSummary() {
        if (rule.timeStart === '') {
            return "Click to set time"
        }
        return rule.timeStart
    }

    MyMultiSelectionDialog {
         id: daysDialog
         titleText: "Days"
         model: backendDaysModel
         acceptButtonText: "OK"
         onAccepted: rule.days = selectedIndexes
     }

    function daysSummary(doNotCapitalize) {
        console.log("DaysSummary called")
        return backendRulesModel.getDaysSummaryText(rule.days);
    }

    function daysEditHandler() {

        daysDialog.selectedIndexes = rule.days

        daysDialog.open();
    }

    MySelectionDialog {
         id: profilesDialog
         titleText: "Select profile"
         model: backendProfilesModel

         onSelectedIndexChanged: {
             if (selectedIndex > -1) {
                 var selectedProfile = model.getProfile(selectedIndex)
                 rule.profile = selectedProfile
                 volumeVisibility();
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

    function volumeVisibility() {
        // IMPROVE this is ugly, enable/disable rule based if profile has volume
        var volumeModel = editRuleModel.get(3)
        if (backendProfilesModel.profileHasVolume(rule.profile)) {
            volumeModel.ruleVisible = true
        } else {
            volumeModel.ruleVisible = false
        }
    }

    // Misc
    function confirmDelete() {
        dConfirmDelete.open()
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
        id: dInvalidRule

        titleText: "This rule can not be used"
        message: "A rule must have days and time set, and profile selected"
        acceptButtonText: "Ok"
    }

    ListModel {
        id: editRuleModel

        ListElement {
            section: "Condition"
            ruleType: "Active days"
            ruleSummaryJS: "daysSummary()"
            clickHandler: "daysEditHandler"
            ruleVisible: true
        }

        ListElement {
            section: "Condition"
            ruleType: "Activated at"
            ruleSummaryJS: "timeStartSummary()"
            clickHandler: "timeStartEditHandler"
            ruleVisible: true
        }

        ListElement {
            section: "Action"
            ruleType: "Set profile"
            ruleSummaryJS: "profileSummary()"
            clickHandler: "profileEditHandler"
            ruleVisible: true
        }

        ListElement {
            section: "Action"
            ruleType: "Set volume"
            ruleSummaryJS: "volumeSummary()"
            clickHandler: "volumeEditHandler"
            ruleVisible: true
        }
    }

    ListView {
        id: listView

        anchors.fill: parent
        pressDelay: 140
        model: editRuleModel

        section.property: "section"
        section.criteria: ViewSection.FullString
        section.delegate: SectionHeader {} // sectionHeading

        header: TextFieldWithLabel {
            labelText: "Rule name"
            placeholderText: "Auto-generated"
            text: rule.ruleName
            height: UIConstants.LIST_ITEM_HEIGHT_SMALL
            onTextChanged: {
                rule.ruleName = text
            }
        }

        delegate:  Item {
            id: listItem
            height: ruleVisible ? UIConstants.LIST_ITEM_HEIGHT_WITH_SUBTITLE : 0
            width: parent.width
            visible: ruleVisible

            Behavior on height {
                NumberAnimation { duration: 300 }
            }

            Row {
                spacing: UIConstants.DEFAULT_MARGIN
                anchors.verticalCenter: parent.verticalCenter

                Item {
                    id: ruleItem
                    width: listItem.width
                    height: ruleVisible ? ruleColumn.height : 0

                    BorderImage {
                        id: background
                        anchors.fill:  ruleItem
                        anchors.margins: -UIConstants.DEFAULT_MARGIN
                        visible: mouseArea.pressed
                        source: !theme.inverted ? "image://theme/meegotouch-list-background-pressed-center"
                                                : "image://theme/meegotouch-list-inverted-background-pressed-center"
                    }

                    Column {
                        id: ruleColumn
                        width: parent.width
                        anchors.verticalCenter: parent.verticalCenter

                        Label {
                            text: ruleType
                            platformStyle: LabelStyleTitle {}
                            font.weight: Font.Bold
                            // font: UiConstants.TitleFont
                        }
                        Label {
                            width: parent.width
                            text: ruleSummaryJS !== "" ? eval(ruleSummaryJS) : ""
                            platformStyle: LabelStyleSubtitle {}
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: background

                        onClicked: {
                            var functionCall = eval(clickHandler)
                            functionCall()
                        }
                    }
                }
            }            
        }
        footer: Text {
            wrapMode: Text.WordWrap
            width: parent.width
            visible: isValidRule()
            font.pixelSize: UIConstants.FONT_SMALL;
            color: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
            text: {
                if (isValidRule()) {
                    return "This rule activates profile " + backendProfilesModel.getProfileToName(rule.profile)
                            + " when clock reaches " + rule.timeStart
                            + " on active days"
                }
                return ""
            }
        }

    }
}
