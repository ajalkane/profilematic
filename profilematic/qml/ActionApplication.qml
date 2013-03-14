/**********************************************************************
 * Copyright 2013 Arto Jalkanen
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
    id: root
    tools: commonTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property RuleAction action
    property int maxApps: 5
    property bool _scanDone: false

    onStatusChanged: {
        // console.log("Status changed ", status)
//        if (status === PageStatus.Activating) {
//            collectingButton.checked = false
//            backendLocation.monitorCellIdChange(true)
//        } else if (status === PageStatus.Deactivating) {
//            backendLocation.monitorCellIdChange(false)
//        }
    }

    ItemSelectionIconDialog {
        id: appSelect
        titleText: qsTr("Select application")
        model: backendApplicationsSortedModel
        onAccepted: {
            if (selectedItem !== null) {
                var launchers = action.application.launchers
                if (launchers.length < root.maxApps) {
                    launchers.push(selectedItem.launcher)
                    action.application.launchers = launchers
                }
            }
        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent

        pressDelay: 140
        clip: true
        contentWidth: parent.width
        contentHeight: container.height // contentItem.childrenRect.height

        Column {
            id: container
            spacing: UIConstants.PADDING_XXLARGE
            anchors.verticalCenter: parent.verticalCenter

            width: parent.width
            height: childrenRect.height

            PageHeader {
                text: qsTr("Application action")
            }

            Button {
                id: selectAppButton
                enabled: action.application.launchers.length < root.maxApps
                text: (action.application.launchers.length < root.maxApps
                       ? qsTr("Add application")
                       : qsTr("Max %1 applications added").arg(root.maxApps))
                onClicked: {
                    if (!_scanDone) {
                        console.log("Start scan")
                        backendApplicationsScanner.scan();
                        console.log("Scan done")
                        _scanDone = true
                    } else {
                        console.log("Skip application scan, already done")
                    }

                    appSelect.selectedIndex = -1
                    appSelect.open()
                }
            } // Button

            SectionHeader {
                width: parent.width
                height: 20
                visible: action.application.launchers.length > 0
                section: qsTr("Applications (%1/%2)").arg(action.application.launchers.length).arg(root.maxApps)

            }

            Repeater {
                model: backendSelectedApplicationsSortedModel

                Item {
                    width: root.width
                    height: Math.max(removeArea.height, appName.height, icon.height) // childrenRect.height // cellId.height // childrenRect.height

                    Image {
                        id: icon
                        source: iconuri
                        width: 80
                        height: 80
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Label {
                        id: appName
                        text: name
                        elide: Text.ElideRight
                        anchors.left: icon.right
                        width: parent.width - removeArea.width - icon.width
                        anchors.leftMargin: UIConstants.DEFAULT_MARGIN
                        anchors.rightMargin: UIConstants.DEFAULT_MARGIN
                        color: theme.inverted ? UIConstants.COLOR_INVERTED_FOREGROUND : UIConstants.COLOR_FOREGROUND
                        // platformStyle: LabelStyleSubtitle {}
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Item {
                        id: removeArea
                        anchors.right: parent.right
                        height: removeImage.height + UIConstants.PADDING_XXLARGE // Math.min(cellId.height, removeImage.height)
                        width: height
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            id: removeBackground
                            anchors.fill: parent
                            visible: removeMouseArea.pressed
                            color: UIConstants.COLOR_SELECT
                        }

                        MouseArea {
                            id: removeMouseArea
                            anchors.fill: parent

                            onClicked: {
                                console.log("Remove clicked")
                                var launchers = action.application.launchers
                                var indexOfLauncher = launchers.indexOf(launcher)
                                if (indexOfLauncher > -1) {
                                    launchers.splice(indexOfLauncher, 1)
                                    action.application.launchers = launchers
                                } else {
                                    console.log("No index found for ", launcher)
                                }
                            }
                        }

                        Image {
                            id: removeImage
                            source: "image://theme/icon-m-toolbar-delete" + (theme.inverted ? "-white" : "")
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                    }
                } // Item
            } // Repeater

            SectionHeader {
                width: parent.width
                height: 20
                section: "Help"
            }

            LabelHelp {
                id: help
                text: qsTr("You can add here applications that are started when the rule becomes active.")
            }

        } // Column
    } // Flickable

    ScrollDecorator {
        flickableItem: flickable
    }

}
