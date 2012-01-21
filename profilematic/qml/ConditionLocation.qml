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

    property Rule    rule;
    property int maxCells: 50

    function addCurrentCell() {
        var cells = rule.locationCells
        var currentCell = backendLocation.currentCell
        if (currentCell >= 0) {
            if (cells.length < maxCells) {
                var xPos = cellIdsFlickable.contentX
                var yPos = cellIdsFlickable.contentY
                cells.push(currentCell)
                rule.locationCells = cells
                cellIdsFlickable.contentX = xPos
                cellIdsFlickable.contentY = yPos
                if (rule.locationCells.length >= maxCells) {
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
            section: "Location by mobile cell identifiers"
        }
    }

    Flickable {
        id: cellIdsFlickable
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
                checkable: rule.locationCells.length < maxCells
                checked: false
                // enabled: backendLocation.currentCell >= 0 && rule.locationCells.length < root.maxCells
                text: (checked ? "Stop collecting"
                               : (rule.locationCells.length < root.maxCells
                                  ? "Start collecting"
                                  : "Max " + root.maxCells + " cells reached"))
                onClicked: {
                    if (checked) {
                        addCurrentCell()
                    }
                }

                Connections {
                    target: root.status == PageStatus.Active ? backendLocation : null
                    onCurrentCellChanged: {
                        console.log("Current cell changed")
                        if (collectingButton.checked) {
                            addCurrentCell()
                        }
                    }
                }
            } // Button

            Label {
                id: summary
                text: backendLocation.currentCell >= 0 ? "Current cell id " + backendLocation.currentCell + ". " + rule.locationCells.length + " included."
                                                       : "Mobile network unreachable"
                width: parent.width

                platformStyle: LabelStyleSubtitle {}
            }

            SectionHeader {
                width: parent.width
                height: 20
                section: "Cell ids"

            }

            Repeater {
                model: rule.locationCells.length // maxCells
                // width: parent.width

                Item {
                    width: root.width
                    height: Math.max(removeArea.height, cellId.height) // childrenRect.height // cellId.height // childrenRect.height

                    Label {
                        id: cellId
                        text: rule.locationCells[index]
                        width: parent.width //  - removeButton.width
                        visible: rule.locationCells.length > index
                        platformStyle: LabelStyleSubtitle {}
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Item {
                        id: removeArea
                        anchors.right: parent.right
                        visible: cellId.visible
                        height: removeImage.height + UIConstants.PADDING_XXLARGE // Math.min(cellId.height, removeImage.height)
                        width: height
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            id: removeBackground
                            anchors.fill: parent
//                            anchors.leftMargin: -parent.anchors.leftMargin - UIConstants.DEFAULT_MARGIN
//                            anchors.rightMargin: -parent.anchors.rightMargin - UIConstants.DEFAULT_MARGIN
                            visible: removeMouseArea.pressed
                            color: UIConstants.COLOR_SELECT
                        }

                        MouseArea {
                            id: removeMouseArea
                            anchors.fill: parent

                            onClicked: {
                                console.log("Cell location Remove clicked")
                                var cells = rule.locationCells
                                cells.splice(index, 1)
                                rule.locationCells = cells
                            }
                        }

                        Image {
                            id: removeImage
                            source: "image://theme/icon-m-toolbar-delete" + (theme.inverted ? "-white" : "")
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                        }

                    }

                    // IMPROVE: use icon
                    // /usr/share/themes/blanco/meegotouch/icons/icon-m-toolbar-delete.png
                    // for inverted:
                    // /usr/share/themes/blanco/meegotouch/icons/icon-m-toolbar-delete-white.png
//                    Button {
//                        id: removeButton
//                        text: "Remove"
//                        anchors.right: parent.right
//                        visible: cellId.visible
//                        width: 256
//                        onClicked: {
//                            console.log("Cell location Remove clicked")
//                            var cells = rule.locationCells
//                            cells.splice(index, 1)
//                            rule.locationCells = cells
//                        }
//                    }
                }
            } // Repeater

            Label {
                id: help
                text: "Location by mobile cell ids doesn't drain batter, unlike using GPS. "
                      + "The downside is that it can not be used for exact location. Typical "
                      + "use case is for recognizing when you're near your home, office, "
                      + "a certain movie theater or similar bounded area."
                      + "\n\n"
                      + "In order to create a rule that matches the area, activate "
                      + "'Start collecting' button. Keep this view open while you're inside "
                      + "the area, and all the cell tower ids your phone connects to will be "
                      + "added. It is good idea to let it collect the cell ids for a while, "
                      + "since in any one place there can be more than one cell id your phone "
                      + "can connect to. You can then save the rule and ProfileMatic will remember "
                      + "to activate the rule when your phone connects to one of the cells."
                      + "\n\n"
                      + "Note: the cell id depends on the network your phone uses. For example, "
                      + "if you use GSM and 3G, you should collect the cell ids using "
                      + "both mobile network modes."
                width: parent.width
                platformStyle: LabelStyleSubtitle {
                    fontPixelSize: UIConstants.FONT_SMALL
                    // textColor: "#8c8c8c"
                    textColor: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                }
            }

        } // Column
    } // Flickable

    ScrollDecorator {
        flickableItem: cellIdsFlickable
    }

}
