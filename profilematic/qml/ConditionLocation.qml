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
    property int maxCells: 3

    Column {
        id: header
        height: childrenRect.height
        spacing: UIConstants.PADDING_XXLARGE

        Button {
            id: addCellButton
            text: enabled ? "Add current cell" : maxCells + " cells supported"
            enabled: rule.locationCells.length < maxCells
            onClicked: {
                console.log("Add current cell clicked")
                var cells = rule.locationCells
                cells.push(backendLocation.currentCell)
                rule.locationCells = cells
            }
        }
        Label {
            id: summary
            text: "Current cell id " + backendLocation.currentCell
            width: parent.width
            platformStyle: LabelStyleSubtitle {}
        }

        SectionHeader {
            section: "Location conditions"
        }
    }

    Flickable {
        anchors.fill: parent
        anchors.topMargin: header.height
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

            Repeater {
                model: maxCells

                Item {
                    width: parent.width
                    height: childrenRect.height

                    Label {
                        id: cellId
                        text: "Cell id " + rule.locationCells[index]
                        width: parent.width - removeButton.width
                        visible: rule.locationCells.length > index
                        platformStyle: LabelStyleSubtitle {}
                        anchors.verticalCenter: removeButton.verticalCenter
                    }

                    // IMPROVE: use icon
                    // /usr/share/themes/blanco/meegotouch/icons/icon-m-toolbar-delete.png
                    // for inverted:
                    // /usr/share/themes/blanco/meegotouch/icons/icon-m-toolbar-delete-white.png
                    Button {
                        id: removeButton
                        text: "Remove"
                        anchors.right: parent.right
                        visible: cellId.visible
                        width: 256
                        onClicked: {
                            console.log("Cell location Remove clicked")
                            var cells = rule.locationCells
                            cells.splice(index, 1)
                            rule.locationCells = cells
                        }
                    }
                }
            }

//            RuleTopicSummary {
//                topic: "Cell id " + rule.locationCells[0]
//                visible: rule.locationCells.length > 0 // isDefaultRule
//            }

//            RuleTopicSummary {
//                topic: "Cell id " + rule.locationCells[1]
//                visible: rule.locationCells.length > 1 // isDefaultRule
//            }

//            RuleTopicSummary {
//                topic: "Cell id " + rule.locationCells[2]
//                visible: rule.locationCells.length > 2 // isDefaultRule
//            }
        }
    }
}
