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
import QtMobility.systeminfo 1.1
import Rule 1.0
import "UIConstants.js" as UIConstants

Page {
    id: root
    tools: commonTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property Rule    rule;
    property int maxWlans: 5

    onStatusChanged: {
        console.log("Status changed ", status)
        if (status == PageStatus.Activating) {
            wlaninfo.monitor(true)
            wlaninfo.setNameIfUsable();
        } else if (status == PageStatus.Deactivating) {
            wlaninfo.monitor(false)
        }
    }

    NetworkInfo {
           id: wlaninfo
           mode: NetworkInfo.WlanMode

//           monitorNameChanges: true
//           monitorModeChanges: true
//           monitorStatusChanges: true

           property string nameIfUsable

           function monitor(doMonitor) {
               monitorNameChanges = doMonitor
               monitorModeChanges = doMonitor
               monitorStatusChanges = doMonitor
           }

           function setNameIfUsable() {
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
        var wlan = rule.wlan
        var currentWlan = wlaninfo.nameIfUsable // TODObackendLocation.currentCell
        if (currentWlan !== '') {
            if (wlan.length < maxWlans) {
                var xPos = wlanNamesFlickable.contentX
                var yPos = wlanNamesFlickable.contentY
                wlan.push(currentWlan)
                rule.wlan = wlan
                wlanNamesFlickable.contentX = xPos
                wlanNamesFlickable.contentY = yPos
                if (rule.wlan.length >= maxWlans) {
                    collectingButton.checked = false
                }
            }
        }
    }

    Column {
        id: header
        width: parent.width
        height: childrenRect.height
        spacing: UIConstants.PADDING_XXLARGE

        SectionHeader {
            width: parent.width
            height: 20
            section: "WLAN"
        }
    }

    Flickable {
        id: wlanNamesFlickable
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

            Button {
                id: collectingButton
                enabled: rule.wlan.length < maxWlans && wlaninfo.nameIfUsable !== ""
                // enabled: backendLocation.currentCell >= 0 && rule.locationCells.length < root.maxCells
                text: (enabled ? "Add current Wlan"
                               : (rule.wlan.length < root.maxWlans
                                  ? "No Wlan"
                                  : "Max " + root.maxWlans + " Wlans added"))
                onClicked: {
                    addCurrentWlan()
                }

                // TODO
//                Connections {
//                    target: root.status == PageStatus.Active ? backendLocation : null
//                    onCurrentCellChanged: {
//                        console.log("Current cell changed")
//                        if (collectingButton.checked) {
//                            addCurrentCell()
//                        }
//                    }
//                }
            } // Button

            Label {
                id: summary
                text: wlaninfo.nameIfUsable !== "" ? "Current Wlan '" + wlaninfo.nameIfUsable + "'": "Not connected to Wlan"
//                text: backendLocation.currentCell >= 0 ? "Current cell id " + backendLocation.currentCell + "."
//                                                       : "Mobile network unreachable"
                width: parent.width

                platformStyle: LabelStyleSubtitle {}
            }

            SectionHeader {
                width: parent.width
                height: 20
                section: "WLAN access points (" + rule.wlan.length + ")"

            }

            Repeater {
                model: rule.wlan.length // maxCells
                // width: parent.width

                Item {
                    width: root.width
                    height: Math.max(removeArea.height, wlanName.height) // childrenRect.height // cellId.height // childrenRect.height

                    Label {
                        id: wlanName
                        text: rule.wlan[index]
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
                                var wlan = rule.wlan
                                wlan.splice(index, 1)
                                rule.wlan = wlan
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

            Label {
                id: help
                text: "<b>TODO help text</b>"

                width: parent.width
                platformStyle: LabelStyleSubtitle {
                    fontPixelSize: UIConstants.FONT_SMALL
                    textColor: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                }
            }

        } // Column
    } // Flickable

    ScrollDecorator {
        flickableItem: wlanNamesFlickable
    }

}
