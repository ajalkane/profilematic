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
import profilematic 1.0
// import QtMobility.systeminfo 1.1
import Rule 1.0
import "UIConstants.js" as UIConstants

Page {
    id: root
    tools: commonTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property RuleCondition condition;
    property int maxWlans: 5

    onStatusChanged: {
        // console.log("Status changed ", status)
        if (status === PageStatus.Activating) {
            wlaninfo.monitor(true)
            wlaninfo.setNameIfUsable();
        } else if (status === PageStatus.Deactivating) {
            wlaninfo.monitor(false)
        }
    }

    NetworkInfo {
           id: wlaninfo
           mode: NetworkInfo.WlanMode

           property string nameIfUsable

           function monitor(doMonitor) {
               monitorNameChanges = doMonitor
               monitorModeChanges = doMonitor
               monitorStatusChanges = doMonitor
           }

           function setNameIfUsable() {
               console.log("ConditionWlan: networkStatus: " + networkStatus)
               console.log("ConditionWlan: mode: " + mode)
               console.log("ConditionWlan: networkName: " + networkName)
               nameIfUsable = (networkStatus == "Connected" && mode == 4 ? networkName : "")
           }

           onNameChanged: {
               console.log("Name changed", networkName)
               setNameIfUsable()
           }
           onModeChanged: {
               console.log("Mode changed", mode)
               setNameIfUsable()
           }
           onStatusChanged: {
               console.log("Network status changed", networkStatus)
               setNameIfUsable()
           }
     }
    function addCurrentWlan() {
        var wlan = condition.wlan
        var currentWlan = wlaninfo.nameIfUsable
        if (currentWlan !== '') {
            if (wlan.length < maxWlans) {
                var xPos = wlanNamesFlickable.contentX
                var yPos = wlanNamesFlickable.contentY
                wlan.push(currentWlan)
                condition.wlan = wlan
                wlanNamesFlickable.contentX = xPos
                wlanNamesFlickable.contentY = yPos
                if (condition.wlan.length >= maxWlans) {
                    collectingButton.checked = false
                }
            }
        }
    }

    Flickable {
        id: wlanNamesFlickable
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
                text: "WLAN condition"
            }

            LabelHelp {
                text: "This condition can be used to activate a rule when connected to WLAN. Add the desired WLAN when connected to it."
            }

            Button {
                id: collectingButton
                enabled: condition.wlan.length < maxWlans && wlaninfo.nameIfUsable !== ""
                // enabled: backendLocation.currentCell >= 0 && conditiong.locationCells.length < root.maxCells
                text: (enabled ? "Add current WLAN"
                               : (condition.wlan.length < root.maxWlans
                                  ? "No WLAN"
                                  : "Max " + root.maxWlans + " WLANs added"))
                onClicked: {
                    addCurrentWlan()
                }
            } // Button

            Label {
                id: summary
                text: wlaninfo.nameIfUsable !== "" ? "Current WLAN '" + wlaninfo.nameIfUsable + "'": "Not connected to WLAN"
                width: parent.width

                platformStyle: LabelStyleSubtitle {}
            }

            SectionHeader {
                width: parent.width
                height: 20
                section: "WLAN access points (" + condition.wlan.length + ")"
            }

            Repeater {
                model: condition.wlan.length // maxCells
                // width: parent.width

                Item {
                    width: root.width
                    height: Math.max(removeArea.height, wlanName.height) // childrenRect.height // cellId.height // childrenRect.height

                    Label {
                        id: wlanName
                        text: condition.wlan[index]
                        width: parent.width //  - removeButton.width
                        platformStyle: LabelStyleSubtitle {}
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
                                var wlan = condition.wlan
                                wlan.splice(index, 1)
                                condition.wlan = wlan
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

            TextFieldWithLabel {
                labelText: "WLAN timeout in seconds"
                placeholderText: "No timeout"
                text: condition.wlanTimeout !== 0 ? condition.wlanTimeout : ""
                inputMethodHints: Qt.ImhDigitsOnly
                validator: RegExpValidator { regExp: /\d{0,3}/ }
                width: parent.width
                onTextChanged: {
                    if (root.status === PageStatus.Active) {
                        condition.wlanTimeout = parseInt(text)
                    }
                }
            }
            LabelHelp {
                text: "Use WLAN timeout if your connection to the WLAN is weak. When timeout is set, "
                      + "WLAN is considered active after losing connection for as many seconds as specified. "
            }
        } // Column
    } // Flickable

    ScrollDecorator {
        flickableItem: wlanNamesFlickable
    }

}
