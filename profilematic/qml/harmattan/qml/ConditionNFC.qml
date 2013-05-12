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
    anchors.leftMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin
    anchors.rightMargin: UIConstants.DEFAULT_MARGIN // UiConstants.DefaultMargin

    property RuleCondition condition;
    property int maxUids: 5

    QueryDialog {
        id: dNfcNotActive

        titleText: qsTr("NFC is not enabled")
        message: qsTr("You have not enabled NFC. To use NFC rules you must enable "
                 + "NFC from device settings. Also you can't collect NFC tags here "
                 + "without enabling NFC tags from the settings.")
        acceptButtonText: qsTr("Ok")
    }


    onStatusChanged: {
        // console.log("Status changed ", status)
        if (status === PageStatus.Activating) {
            backendNfc.monitorNfc(true)
            if (!backendNfc.isAvailable()) {
                console.log("Opening NFC not active warning dialog")
                dNfcNotActive.open();
            } else {
                console.log("NFC is active, no warning dialog")
            }
        } else if (status === PageStatus.Deactivating) {
            backendNfc.monitorNfc(false)
        }
    }

    function addNfcUid(nfcUid) {
        console.log("addNfcUid('" + nfcUid + ")")
        if (nfcUid !== '') {
            var uids = condition.nfc.uids

            if (uids.length < maxUids) {
                var xPos = flickable.contentX
                var yPos = flickable.contentY
                uids.push(nfcUid)
                console.log("Adding nfcUid " + nfcUid)
                condition.nfc.uids = uids
                flickable.contentX = xPos
                flickable.contentY = yPos
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
                text: qsTr("NFC condition")
            }

            LabelHelp {
                text: qsTr("Bring NFC tag close to the phone to add it to tags that are matched by this rule")
            }

            Label {
                id: summary
                text: ""
                width: parent.width
                visible: text !== ""

                platformStyle: LabelStyleSubtitle {}

                Connections {
                    target: backendNfc
                    onCurrentNfcUidChanged: {
                        summary.text = qsTr("Last detected NFC id: %1").arg(uid)
                        addNfcUid(uid)
                    }
                }
            }

            SectionHeader {
                width: parent.width
                height: 20
                visible: condition.nfc.uids.length > 0
                section: qsTr("NFC tags (%1/%2)").arg(condition.nfc.uids.length).arg(maxUids)

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
                section: qsTr("Condition behaviour")
            }

            Item {
                id: nfcBehaviourContainer
                width: parent.width
                height: toggleCondition.height

                RuleTopicSummary {
                    id: toggleCondition
                    topic: qsTr("Toggle condition")
                    topicWidth: parent.width - toggleSwitch.width
                    summary: toggleSwitch.checked ? qsTr("Condition active toggled on/off with NFC tag")
                                                  : qsTr("Condition active only when NFC tag detected")
                    onTopicClicked: toggleSwitch.checked = !toggleSwitch.checked
                }
                Switch {
                    id: toggleSwitch
                    checked: condition.nfc.toggleCondition
                    anchors.right: nfcBehaviourContainer.right // container.right
                    anchors.top: parent.top
                    anchors.verticalCenter: parent.top
                    onCheckedChanged: {
                        condition.nfc.toggleCondition = checked
                    }
                }
            }
        } // Column
    } // Flickable

    ScrollDecorator {
        flickableItem: flickable
    }

}
