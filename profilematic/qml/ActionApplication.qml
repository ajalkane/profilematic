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

    property RuleAction action;
    property int maxApps: 10

    onStatusChanged: {
        // console.log("Status changed ", status)
//        if (status === PageStatus.Activating) {
//            collectingButton.checked = false
//            backendLocation.monitorCellIdChange(true)
//        } else if (status === PageStatus.Deactivating) {
//            backendLocation.monitorCellIdChange(false)
//        }
    }

    MyMultiSelectionIconDialog {
        id: appSelect
        acceptButtonText: "OK"
        platformStyle: SelectionDialogStyle {
            itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
        }

        model: ListModel {
            ListElement {
                name: "Application Name 1"
                iconuri: "file:///usr/share/icons/asunder.png"
            }
            ListElement {
                name: "Application Name 2"
                iconuri: "file:///usr/share/icons/cab_extract.png"
            }
            ListElement {
                name: "Application Name 3"
                iconuri: "file:///usr/share/icons/cab_view.png"
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
                text: "Application action"
            }

            Button {
                id: selectAppButton
                // TODO
                // checkable: condition.locationCells.length < maxCells
                // checked: false
                // enabled: backendLocation.currentCell >= 0 && condition.locationCells.length < root.maxCells
                // TODO
                text: "Select applications"
//                      (checked ? "Add application"
//                               : (condition.locationCells.length < root.maxCells
//                                  ? "Start collecting"
//                                  : "Max " + root.maxApps + " applications added"))
                onClicked: {
                    appSelect.open()
                    if (checked) {
                        // addCurrentCell()
                    }
                }
            } // Button

            SectionHeader {
                width: parent.width
                height: 20
                // TODO
                section: "Applications"
                // section: "Applications (" + condition.locationCells.length + ")"

            }

            Repeater {
                model: ListModel {
                    ListElement {
                        name: "Application Name 1"
                        iconuri: "file:///usr/share/icons/asunder.png"
                    }
                    ListElement {
                        name: "Application Name 2"
                        iconuri: "file:///usr/share/icons/cab_extract.png"
                    }
                    ListElement {
                        name: "Application Name 3"
                        iconuri: "file:///usr/share/icons/cab_view.png"
                    }
                }

                Item {
                    width: root.width
                    height: Math.max(removeArea.height, appName.height) // childrenRect.height // cellId.height // childrenRect.height

                    Image {
                        id: icon
                        source: iconuri
                        width: 80
                        height: 80
                    }

                    Label {
                        id: appName
                        text: name
                        anchors.left: icon.right
                        width: parent.width //  - removeButton.width
                        // TODO
                        // visible: condition.locationCells.length > index
                        platformStyle: LabelStyleSubtitle {}
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Item {
                        id: removeArea
                        anchors.right: parent.right
                        // TODO
                        visible: appName.visible
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
                                console.log("Cell location Remove clicked")
                                // TODO
//                                var cells = condition.locationCells
//                                cells.splice(index, 1)
//                                condition.locationCells = cells
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
                text: "Help bla bla bla"
            }

        } // Column
    } // Flickable

    ScrollDecorator {
        flickableItem: flickable
    }

}
