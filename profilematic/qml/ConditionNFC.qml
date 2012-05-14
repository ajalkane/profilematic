/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
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

    property RuleCondition condition;
    property int maxUids: 5

    onStatusChanged: {
//        // console.log("Status changed ", status)
//        if (status === PageStatus.Activating) {
//            collectingButton.checked = false
//            backendLocation.monitorCellIdChange(true)
//        } else if (status === PageStatus.Deactivating) {
//            backendLocation.monitorCellIdChange(false)
//        }
    }

//    function addCurrentCell() {
//        var cells = condition.locationCells
//        var currentCell = backendLocation.currentCell
//        if (currentCell >= 0) {
//            if (cells.length < maxCells) {
//                var xPos = cellIdsFlickable.contentX
//                var yPos = cellIdsFlickable.contentY
//                cells.push(currentCell)
//                condition.locationCells = cells
//                cellIdsFlickable.contentX = xPos
//                cellIdsFlickable.contentY = yPos
//                if (condition.locationCells.length >= maxCells) {
//                    collectingButton.checked = false
//                }
//            }
//        }
//    }

    Column {
        id: header
        width: parent.width
        height: childrenRect.height
        spacing: UIConstants.PADDING_XXLARGE

        SectionHeader {
            width: parent.width
            height: 20
            section: "NFC condition"
        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        anchors.topMargin: header.height + UIConstants.PADDING_XXLARGE

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

            LabelHelp {
                text: "Bring NFC tag close to the phone to add it to tags that are matched by this rule"
            }

            Label {
                id: summary
                text: "Placeholder" // backendLocation.currentCell >= 0 ? "Current cell id " + backendLocation.currentCell + "."
                                    //                   : "Mobile network unreachable"
                width: parent.width

                platformStyle: LabelStyleSubtitle {}
            }

            SectionHeader {
                width: parent.width
                height: 20
                section: "NFC tags (" + condition.nfc.uids.length + ")"

            }

            Repeater {
                model: condition.nfc.uids.length // maxCells
                // width: parent.width

                Item {
                    width: root.width
                    height: Math.max(removeArea.height, uid.height) // childrenRect.height // cellId.height // childrenRect.height

                    Label {
                        id: uid
                        text: condition.nfc.uids[index]
                        width: parent.width //  - removeButton.width
                        visible: condition.nfc.uids.length > index
                        platformStyle: LabelStyleSubtitle {}
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Item {
                        id: removeArea
                        anchors.right: parent.right
                        visible: uid.visible
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
                                console.log("NFC uid remove clicked")
                                var uids = condition.nfc.uids
                                uids.splice(index, 1)
                                condition.nfc.uids = uids
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

//            TextFieldWithLabel {
//                labelText: "Timeout in seconds"
//                placeholderText: "No timeout"
//                text: condition.locationCellsTimeout !== 0 ? condition.locationCellsTimeout : ""
//                inputMethodHints: Qt.ImhDigitsOnly
//                validator: RegExpValidator { regExp: /\d{0,3}/ }
//                width: parent.width
//                onTextChanged: {
//                    if (root.status === PageStatus.Active) {
//                        condition.locationCellsTimeout = parseInt(text)
//                    }
//                }
//            }

            SectionHeader {
                width: parent.width
                height: 20
                section: "Condition behaviour"
            }

            Item {
                id: nfcBehaviourContainer
                width: parent.width
                height: toggleCondition.height

                RuleTopicSummary {
                    id: toggleCondition
                    topic: "Toggle condition"
                    topicWidth: parent.width - toggleSwitch.width
                    summary: toggleSwitch.checked ? "Condition active toggled on/off with NFC tag"
                                                  : "Condition active only when NFC tag detected"
                    onTopicClicked: toggleSwitch.checked = !toggleSwitch.checked
                }
                Switch {
                    id: toggleSwitch
                    // checked: action.restoreProfile
                    anchors.right: nfcBehaviourContainer.right // container.right
                    anchors.top: parent.top
                    anchors.verticalCenter: parent.top
                    //onCheckedChanged: {
                        // action.restoreProfile = checked
                    //}
                }
            }
        } // Column
    } // Flickable

    ScrollDecorator {
        flickableItem: flickable
    }

}
