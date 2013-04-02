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
import Rule 1.0
import "UIConstants.js" as UIConstants

Page {
    id: showRules
    tools: !backendRulesModel.backendError ? showRuleTools : null
    anchors.margins: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin

    property Item editRule
    property int  editRuleModelIndex: -1

    // Loading a QML file dynamically is slow, so an animation
    // is shown first to give user feedback something is happening.
    property int _editRuleDelay: 200

    function loadEditRule(properties) {
        return instantiateComponent("EditRule.qml", properties)
    }

    function loadAboutDialog() {
        return instantiateComponent("AboutDialog.qml", {})
    }

    function instantiateComponent(file, properties) {
        console.log("createComponent", file)
        var component = Qt.createComponent(file)        
        console.log("/createComponent", file)

        if (component.status === Component.Ready) {
            console.log("createObject", file)
            var comp = component.createObject(showRules, properties)
            console.log("/createObject")
            return comp
        }
        else
            console.log("Error loading component:", component.errorString());

    }

    ToolBarLayout {
        id: showRuleTools
        visible: !backendRulesModel.backendError

        ToolIcon {
            iconId: "toolbar-add"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: { openNewRule() }
        }
        ToolIcon {
            iconId: "toolbar-view-menu";
            anchors.right: parent.right

            onClicked: (mainMenu.status === DialogStatus.Closed) ? mainMenu.open() : mainMenu.close()

        }
    }

    Menu {
        id: mainMenu
        MenuLayout {
            MenuItem {
                text: backendRulesModel.active ? qsTr("Deactivate rules") : qsTr("Activate rules")
                onClicked: {
                    console.log("Switching rule watching to", !backendRulesModel.active)
                    backendRulesModel.active = !backendRulesModel.active
                }
            }
            MenuItem { text: qsTr("About"); onClicked: loadAboutDialog().open() }
        }
    }

    ApplicationHeader {
        id: appHeader
        title: backendRulesModel.active ? "ProfileMatic" : "ProfileMatic" + " " + qsTr("(inactive)")
        // This is the same color as in Harmattan pulldown Profile background color
        backgroundColor: UIConstants.COLOR_APP_HEADER
        titleColor: "white"
        anchors.leftMargin: -UIConstants.DEFAULT_MARGIN
        anchors.rightMargin: -UIConstants.DEFAULT_MARGIN
        anchors.topMargin: -UIConstants.DEFAULT_MARGIN
        z: 100

        states: State {
            name: "disabled"
            when: !backendRulesModel.active
            PropertyChanges {
                target: appHeader
                backgroundColor: "black"
            }
        }
        transitions: Transition {
            from: ""
            to: "disabled"
            reversible: true
            ColorAnimation { duration: 1000 }
        }
    }

    Rectangle {
        id: disabledIndicator
        width: parent.widht
        height: 0
        anchors.rightMargin: -UIConstants.DEFAULT_MARGIN
        color: "black"
        anchors.top: appHeader.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: -UIConstants.DEFAULT_MARGIN
        anchors.topMargin: -height
        visible: color !== "black"
        z: 101

        states: State {
            name: "disabled"
            when: !backendRulesModel.active
            PropertyChanges {
                target: disabledIndicator
                color: "red"
                height: 6
            }
        }
        transitions: Transition {
            from: ""
            to: "disabled"
            reversible: true
            ParallelAnimation {
                NumberAnimation { properties: "height"; duration: 1000; easing.type: Easing.InCubic }
                ColorAnimation { duration: 1000; easing.type: Easing.InCubic  }
            }
        }
    }

    Text {
        wrapMode: Text.WordWrap
        anchors.top: appHeader.bottom
        anchors.centerIn: parent
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        // There's always default rule, so display when count <= 1
        visible: !backendRulesModel.backendError && backendRulesModel.count <= 1
        font.pixelSize: UIConstants.FONT_XXXLARGE;
        color: mouseAreaFirstRule.containsMouse ? (!theme.inverted ? UIConstants.COLOR_FOREGROUND : UIConstants.COLOR_INVERTED_FOREGROUND)
                                 : (!theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND)
        text: qsTr("Start using automatic profiles by creating the first rule")

        MouseArea {
            id: mouseAreaFirstRule
            anchors.fill: parent

            onClicked: {
                console.log("No rules, clicked on text")
                openNewRule()
            }
        }
    }

    Text {
        wrapMode: Text.WordWrap
        anchors.top: appHeader.bottom
        anchors.centerIn: parent
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        visible: backendRulesModel.backendError
        font.pixelSize: UIConstants.FONT_XLARGE;
        color: "red"
        text: qsTr("Oops. Still starting my engines. I'll be up and running soon, close me and come back in 2 minutes. If problem persists, reboot the phone or reinstall ProfileMatic.")
    }

    ListView {
        id: listView
        // There's always default rule, so display when count > 1
        visible: backendRulesModel.count > 1
        anchors {
            top: appHeader.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: UIConstants.PADDING_XLARGE
        }
        model: backendRulesModel
        pressDelay: 140
        currentIndex: -1

        highlight: Rectangle {
            id: highlightBar
            color: UIConstants.COLOR_SELECT
            opacity: 0.0

            states: State {
                name: "Selected"
                when: listView.currentItem !== -1
                PropertyChanges { target: highlightBar; opacity: 0.8 }
            }

            transitions: Transition {
                NumberAnimation {
                    property: "opacity"
                    duration: _editRuleDelay
                }

            }
        }

// Commented out, as I hope explicit text is not needed after the new visuals
//        header: Item {
//            visible: !backendRulesModel.active
//            height: !backendRulesModel.active ? childrenRect.height : 0
//            width: parent.width
//            Text {
//                    wrapMode: Text.WordWrap
//                    width: parent.width
//                    font.pixelSize: UIConstants.FONT_LARGE;
//                    color: "red"
//                    text: "Warning: Rules are not followed! Activate from menu."
//            }
//        }

        delegate: Row {
            id: listItem
            width: parent.width
            Item {
                id: upColumn
                anchors.verticalCenter: parent.verticalCenter
                width: childrenRect.width
                height:  childrenRect.height
                // z-order is 1, so that rule selection highlight does not cover up arrow
                z: 1

                Item {
                    width: childrenRect.width
                    height: up.height
                    // Do not show up arrow for first or defaultRule
                    visible: index > 0 && index < listView.count - 1
                    Rectangle {
                        id: backgroundUp
                        anchors.fill: up
                        anchors.margins: -UIConstants.DEFAULT_MARGIN
                        visible: mouseAreaUp.pressed
                        color: UIConstants.COLOR_SELECT
                    }

                    MouseArea {
                        id: mouseAreaUp
                        anchors.fill: up

                        onClicked: {
                            console.log("Clicked up " + model, index)
                            backendRulesModel.moveRule(index, index - 1)
                        }
                    }

                    Image {
                        id: up
                        source: "image://theme/icon-m-toolbar-up" + (theme.inverted ? "-white" : "")
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            } // Item upColumn

            Item {
                id: ruleItem

                anchors.leftMargin: upColumn.width + parent.spacing
                anchors.rightMargin: downColumn.width + parent.spacing
                width: listItem.width - (upColumn.width + parent.spacing) - (downColumn.width + parent.spacing)
                height:  rule.height + UIConstants.PADDING_XLARGE

                // Color bar for rules that are currently active
                Rectangle {
                    width: 5
                    height: parent.height
                    color: UIConstants.COLOR_APP_HEADER
                    // To avoid confusion, default rule is shown matching only when it has some action(s)
                    visible: matching && (!isDefaultRule || ruleSummary !== "When other rules don't apply")
                    x: -width - 10
                }

                Rectangle {
                    id: background
                    anchors.fill: parent
                    anchors.leftMargin: -parent.anchors.leftMargin - UIConstants.DEFAULT_MARGIN
                    anchors.rightMargin: -parent.anchors.rightMargin - UIConstants.DEFAULT_MARGIN
                    visible: mouseAreaRule.pressed
                    color: UIConstants.COLOR_SELECT
                }
                Column {
                    id: rule
                    anchors.verticalCenter: parent.verticalCenter

                    Label {
                        id: mainText
                        text: isDefaultRule ? qsTr("Default rule") : model.ruleName
                        width: parent.width
                        platformStyle: LabelStyleTitle {
                            disabled: !ruleActive
                            // IMPROVE: "orange" to UIConstants
                            textColor: model.stopIfMatched ? "orange" : implicitTextColor
                        }
                        font.weight: Font.Bold
                        elide: Text.ElideRight
                    }
                    Label {
                        width: ruleItem.width
                        text: ruleSummary
                        platformStyle: LabelStyleSubtitle {
                            disabled: !ruleActive;
                            fontPixelSize: UIConstants.FONT_SMALL
                        }
                        wrapMode: Text.WordWrap
                    }
                }

                MouseArea {
                    id: mouseAreaRule
                    anchors.fill: rule

                    onClicked: {
                        if (editRuleModelIndex > -1) {
                            console.log("Already opening rule for editing, overriding click")
                        }
                        editRuleModelIndex = index
                        console.log("onClicked parent.state", parent.state)
                        loadEditRuleTimer.start()
                        listView.currentIndex = index
                    }
                    onPressAndHold: {
                        contextMenu.openForIndex(index, index === listView.count - 1, ruleActive)
                    }
                }

                Item {
                    id: downColumn
                    anchors.left:  parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: childrenRect.width
                    height: down.height
                    // Do not show down arrow for rule preceding defaultRule, which is always the last item
                    visible: index < listView.count - 2

                    Rectangle {
                        id: backgroundDown
                        anchors.fill: down
                        anchors.margins: -UIConstants.DEFAULT_MARGIN
                        visible: mouseAreaDown.pressed
                        color: UIConstants.COLOR_SELECT
                    }

                    MouseArea {
                        id: mouseAreaDown
                        anchors.fill: backgroundDown

                        onClicked: {
                            console.log("Clicked down " + model, index)
                            // IMPROVE: animation doesn't seem to be easily doable currently on ListView on move, see
                            // http://lists.qt.nokia.com/pipermail/qt-qml/2010-March/000035.html
                            backendRulesModel.moveRule(index, index + 1)
                        }
                    }

                    Image {
                        id: down
                        source: "image://theme/icon-m-toolbar-down" + (theme.inverted ? "-white" : "")
                        // Down arrow is positioned at bottom, so that long rule names have more space for them
                        anchors.verticalCenter: parent.bottom
                    }
                }

                Timer {
                    id: loadEditRuleTimer
                    interval: _editRuleDelay
                    onTriggered: {
                        if (editRuleModelIndex !== index) {
                            console.log("Already opening another editRule, canceling this one. Opening/this",
                                        editRuleModelIndex, index)
                            return
                        }

                        backendRulesModel.setEditRule(index)
                        __editRule();
                        listView.currentIndex = -1
                    }
                }
            }
        }

        footer: Column {
            width: parent.width
            spacing: UIConstants.PADDING_SMALL

            LabelHelp {
                visible: backendRulesModel.count > 1
                text: qsTr("Blue bar is shown on rules whose conditions match currently. First matching rule is preferred for an action if there are conflicts. "
                    + "Use arrows to change the order of the rules.")
            }
            LabelHelp {
                width: parent.width
                // No rules are displayed when only default rule (without user specified rules)
                visible: backendRulesModel.count > 1
                text: backendRulesModel.active ? qsTr("Rules are followed even if application is closed or device is rebooted") : ""
            }
        }
    }

    Connections {
        target: editRule
        onSaved: setRule()
        onDeleted: deleteRule(editRuleModelIndex)
    }

    QueryDialog {
        id: dConfirmDelete

        titleText: qsTr("Delete this rule?")
        acceptButtonText: qsTr("Delete")
        rejectButtonText: qsTr("Cancel")

        onAccepted: {
            deleteRule(contextMenu.selectedRuleIndex)
        }
    }

    ContextMenu {
        id: contextMenu
        property int selectedRuleIndex
        property int isDefaultRule
        property bool ruleActive

        MenuLayout {
            MenuItem {
                text: qsTr("Copy as new")
                onClicked: copyAsNewRule(contextMenu.selectedRuleIndex)
            }
            MenuItem {
                text: qsTr("Delete")
                onClicked: dConfirmDelete.open()
                enabled: !contextMenu.isDefaultRule
            }
            MenuItem {
                text: qsTr("Run rule's actions")
                onClicked: backendRulesModel.executeAction(contextMenu.selectedRuleIndex)
            }
            MenuItem {
                text: contextMenu.ruleActive ? qsTr("Deactivate rule") : qsTr("Activate rule")
                onClicked: backendRulesModel.toggleRuleActive(contextMenu.selectedRuleIndex)
                enabled: !contextMenu.isDefaultRule
            }
        }
        function openForIndex(index, isDefaultRule, ruleActive) {
            selectedRuleIndex = index
            contextMenu.isDefaultRule = isDefaultRule
            contextMenu.ruleActive = ruleActive
            open()
        }
    }

    function setRule() {
        backendRulesModel.saveEditRule()
    }

    function __editRule() {
        var p = {
            "rule": backendRulesModel.getEditRule()
        }
        backendConditionEditModel.initializeEdit();
        backendActionEditModel.initializeEdit();
        backendSelectedApplicationsModel.initializeEdit();

        editRule = loadEditRule(p)
        console.log("Opening rule");
        pageStack.push(editRule)
    }

    function __openNewRule() {
        editRuleModelIndex = -1
        __editRule();
    }

    function openNewRule() {
        console.log("openNewRule")

        backendRulesModel.setNewEditRule()
        __openNewRule()
    }

    function copyAsNewRule(index) {
        console.log("copyAsNewRule")

        backendRulesModel.setNewEditRuleFrom(index)
        __openNewRule()
    }

    function deleteRule(index) {
        // IMPROVE use ruleId: deleteRule(ruleId)
        if (index >= 0) {
            backendRulesModel.removeRule(index)
        }
    }

    ScrollDecorator {
        flickableItem: listView
    }

}
