/*********************************************************************
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

import "MyMultiSelectionDialog.js" as MultiSelectionDialog
import "UIConstants.js" as UI

MyMultiSelectionDialog {
    id: root

    delegate: Component {
        id: defaultDelegate

        Item {
            id: delegateItem

            height: root.platformStyle.itemHeight
            anchors.left: parent.left
            anchors.right: parent.right

            MouseArea {
                id: delegateMouseArea
                anchors.fill: parent;
                onPressed: MultiSelectionDialog.__toggleIndex(index);
            }

            Rectangle {
                id: backgroundRect
                anchors.fill: parent
                color: MultiSelectionDialog.__isSelected(index) ? root.platformStyle.itemSelectedBackgroundColor : root.platformStyle.itemBackgroundColor
            }

            BorderImage {
                id: background
                anchors.fill: parent
                border { left: UI.CORNER_MARGINS; top: UI.CORNER_MARGINS; right: UI.CORNER_MARGINS; bottom: UI.CORNER_MARGINS }
                source: delegateMouseArea.pressed ? root.platformStyle.itemPressedBackground :
                        MultiSelectionDialog.__isSelected(index) ? root.platformStyle.itemSelectedBackground :
                        root.platformStyle.itemBackground
            }
            Image {
                id: itemIcon
                source: iconuri
                width: 80
                height: 80
            }

            Label {
                id: itemText
                elide: Text.ElideRight
                color: MultiSelectionDialog.__isSelected(index) ? root.platformStyle.itemSelectedTextColor : root.platformStyle.itemTextColor
                anchors.verticalCenter: delegateItem.verticalCenter
                anchors.left: itemIcon.right
                anchors.right: parent.right
                anchors.leftMargin: root.platformStyle.itemLeftMargin
                anchors.rightMargin: root.platformStyle.itemRightMargin
                text: name;
                font: root.platformStyle.itemFont
            }
        }
    }
}

//Sheet {
//    id: root
//    acceptButtonText: "Ok"
//    rejectButtonText: "Cancel"
//    anchors.margins: UIConstants.DEFAULT_MARGIN

//    content: Flickable {
//        id: flickable
//        anchors.fill: parent

//        pressDelay: 140
//        clip: true
//        contentWidth: parent.width
//        contentHeight: container.height // contentItem.childrenRect.height

//        Column {
//            id: container
//            spacing: UIConstants.PADDING_XXLARGE
//            anchors.verticalCenter: parent.verticalCenter

//            width: parent.width
//            height: childrenRect.height

//            Repeater {
//                model: ListModel {
//                    ListElement {
//                        appid: "1"
//                        name: "Application Name 1"
//                        iconuri: "file:///usr/share/icons/asunder.png"
//                    }
//                    ListElement {
//                        appid: "2"
//                        name: "Application Name 2"
//                        iconuri: "file:///usr/share/icons/cab_extract.png"
//                    }
//                    ListElement {
//                        appid: "3"
//                        name: "Application Name 3"
//                        iconuri: "file:///usr/share/icons/cab_view.png"
//                    }
//                }

//                Item {
//                    width: root.width
//                    height: Math.max(icon.height, appName.height) // childrenRect.height // cellId.height // childrenRect.height

//                    Image {
//                        id: icon
//                        source: iconuri
//                        width: 80
//                        height: 80
//                    }

//                    Label {
//                        id: appName
//                        text: name
//                        anchors.left: icon.right
//                        width: parent.width //  - removeButton.width
//                        // TODO
//                        // visible: condition.locationCells.length > index
//                        platformStyle: LabelStyleSubtitle {}
//                        anchors.verticalCenter: parent.verticalCenter
//                    }
//                } // Item
//            } // Repeater
//        }  // Column
//    } // Flickable
////    ScrollDecorator {
////        flickableItem: flickable
////    }

//}
