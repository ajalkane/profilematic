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

        if (component.status == Component.Ready) {
            console.log("createObject", file)
            var comp = component.createObject(showRules, properties)
            console.log("/createObject", file)
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

            onClicked: (mainMenu.status == DialogStatus.Closed) ? mainMenu.open() : mainMenu.close()

        }
    }

    Menu {
        id: mainMenu
        MenuLayout {
            MenuItem {
                text: backendRulesModel.active ? "Stop following rules" : "Start following rules"
                onClicked: {
                    console.log("Switching rule watching to", !backendRulesModel.active)
                    backendRulesModel.active = !backendRulesModel.active
                }
            }
            MenuItem { text: "About"; onClicked: loadAboutDialog().open() }
        }
    }

    ApplicationHeader {
        id: appHeader
        title: "ProfileMatic"
        // This is the same color as in Harmattan pulldown Profile background color
        backgroundColor: "#428eff"
        titleColor: "white"
        anchors.leftMargin: -UIConstants.DEFAULT_MARGIN
        anchors.rightMargin: -UIConstants.DEFAULT_MARGIN
        anchors.topMargin: -UIConstants.DEFAULT_MARGIN
    }

    Text {
        wrapMode: Text.WordWrap
        anchors.top: appHeader.bottom
        anchors.centerIn: parent
        width: parent.width
        horizontalAlignment: Text.AlignHCenter
        visible: !backendRulesModel.backendError && backendRulesModel.count === 0
        font.pixelSize: UIConstants.FONT_XXXLARGE;
        color: mouseAreaFirstRule.containsMouse ? (!theme.inverted ? UIConstants.COLOR_FOREGROUND : UIConstants.COLOR_INVERTED_FOREGROUND)
                                 : (!theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND)
        text: "Add your first rule"

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
        text: "Oops. Still starting my engines. I'll be up and running soon, close me and come back in 2 minutes. If problem persists, reboot the phone or reinstall ProfileMatic."
    }

    ListView {
        id: listView
        visible: backendRulesModel.count > 0
        anchors {
            top: appHeader.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        model: backendRulesModel
        pressDelay: 140
        currentIndex: -1

        highlight: Rectangle {
            id: highlightBar
            color: "#428eff"
            opacity: 0.0

            states: State {
                name: "Selected"
                when: listView.currentItem != -1
                PropertyChanges { target: highlightBar; opacity: 0.8 }
            }

            transitions: Transition {
                NumberAnimation {
                    property: "opacity"
                    duration: _editRuleDelay
                }

            }
        }

        header: Item {
            visible: !backendRulesModel.active
            height: !backendRulesModel.active ? childrenRect.height : 0
            width: parent.width
            Text {
                    wrapMode: Text.WordWrap
                    width: parent.width
                    font.pixelSize: UIConstants.FONT_LARGE;
                    color: "red"
                    text: "Warning: Rules are not followed! Activate from menu."
            }
        }

        delegate:  Item {
            id: listItem
            height: UIConstants.LIST_ITEM_HEIGHT_WITH_SUBTITLE
            width: parent.width

            Row {
                anchors.fill: parent

                Item {
                    id: upColumn
                    anchors.verticalCenter: parent.verticalCenter
                    width: childrenRect.width
                    height:  parent.height

                    Item {
                        width: childrenRect.width
                        height:  parent.height
                        visible: index > 0
                        BorderImage {
                            id: backgroundUp
                            anchors.fill: up
                            anchors.margins: -UIConstants.DEFAULT_MARGIN
                            visible: mouseAreaUp.pressed
                            source: !theme.inverted ? "image://theme/meegotouch-list-background-pressed"
                                                    : "image://theme/meegotouch-list-inverted-background-pressed"
                        }

                        MouseArea {
                            id: mouseAreaUp
                            anchors.fill: backgroundUp

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
                    width: listItem.width - (upColumn.width + parent.spacing) - (downColumn.width + parent.spacing) // childrenRect.width
                    height:  parent.height

                    BorderImage {
                        id: background
                        anchors.fill: parent
                        anchors.leftMargin: -parent.anchors.leftMargin - UIConstants.DEFAULT_MARGIN
                        anchors.rightMargin: -parent.anchors.rightMargin - UIConstants.DEFAULT_MARGIN
                        visible: mouseAreaRule.pressed
                        source: !theme.inverted ? "image://theme/meegotouch-list-background-pressed-center"
                                                : "image://theme/meegotouch-list-inverted-background-pressed-center"
                    }
                    Column {
                        id: rule
                        anchors.verticalCenter: parent.verticalCenter

                        Label {
                            id: mainText
                            text: model.ruleName
                            width: parent.width
                            platformStyle: LabelStyleTitle {}
                            font.weight: Font.Bold
                            elide: Text.ElideRight
                        }
                        Label {
                            width: ruleItem.width
                            text: {
                                console.log("Called summary label for index", index)
                                var summary = backendProfilesModel.getProfileToName(profile) + ". ";
                                var fm = flightMode
                                switch (fm) {
                                case 0: summary += "Flight mode off. "; break;
                                case 1: summary += "Flight mode on. "; break;
                                }

                                summary += daysSummary + ". Starts at " + timeStart
                                return summary;
                            }
                            platformStyle: LabelStyleSubtitle {}
                            wrapMode: Text.WordWrap
                        }
                    }

                    MouseArea {
                        id: mouseAreaRule
                        anchors.fill: parent // background

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
                            contextMenu.openForIndex(index)
                        }
                    }

                    Item {
                        id: downColumn
                        anchors.left:  parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        width: childrenRect.width
                        height:  parent.height
                        visible: index < listView.count - 1

                        BorderImage {
                            id: backgroundDown
                            anchors.fill: down
                            anchors.margins: -UIConstants.DEFAULT_MARGIN
                            visible: mouseAreaDown.pressed
                            source: !theme.inverted ? "image://theme/meegotouch-list-background-pressed"
                                                    : "image://theme/meegotouch-list-inverted-background-pressed"
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
                            anchors.verticalCenter: parent.verticalCenter
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
                            var p = {
                                "rule": backendRulesModel.getEditRule()
                            }
                            console.log("loadEditRule")
                            console.log("loadEditRule p.rule", p.rule)
                            editRule = loadEditRule(p)
                            console.log("/loadEditRule")
                            pageStack.push(editRule)
                            listView.currentIndex = -1
                        }

                    }
                }
            }
        }
        footer: Column {
            width: parent.width
            spacing: UIConstants.PADDING_SMALL

            Text {
                wrapMode: Text.WordWrap
                width: parent.width
                visible: backendRulesModel.count > 1
                font.pixelSize: UIConstants.FONT_SMALL;
                color: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                text: "Use arrows to change the order of the rules. The first rule that matches is used to set the profile.";
            }
            Text {
                wrapMode: Text.WordWrap
                width: parent.width
                visible: backendRulesModel.count > 0
                font.pixelSize: UIConstants.FONT_SMALL;
                color: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                text: backendRulesModel.active ? "Rules are followed even if application is closed or device is rebooted" : ""
            }
        }
    }

    Connections {
        target: editRule
        onSaved: setRule(editRuleModelIndex)
        onDeleted: deleteRule(editRuleModelIndex)
    }

    ContextMenu {
        id: contextMenu
        property int deleteIndex

        MenuLayout {
            MenuItem {
                text: "Delete"
                onClicked: deleteRule(contextMenu.deleteIndex)
            }
        }
        function openForIndex(index) {
            deleteIndex = index
            open()
        }
    }

    function setRule(index) {
        backendRulesModel.saveEditRule()
    }

    function openNewRule() {
        console.log("openNewRule")
        backendRulesModel.setNewEditRule();
        var p = {
            "rule": backendRulesModel.getEditRule()
        }
        editRuleModelIndex = -1
        editRule = loadEditRule(p)
        pageStack.push(editRule)
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
