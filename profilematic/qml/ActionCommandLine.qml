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

    property RuleAction action;

    Flickable {
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
                text: qsTr("Custom action")
            }

            LabelHelp {
                text: qsTr("Use this action only if you know what you're doing (ie. you're familiar with using terminal)!")
            }

            SectionHeader {
                section: qsTr("When rule activates")
            }

            TextFieldWithLabel {
                labelText: qsTr("Command to run")
                placeholderText: qsTr("No custom action")
                text: action.commandLine
                // height: UIConstants.LIST_ITEM_HEIGHT_SMALL
                width: parent.width
                onTextChanged: {
                    action.commandLine = text
                }
            }

            LabelHelp {
                text: qsTr("Enter into the text box the shell command you want to run when this rule is activated. "
                      + "Always use full paths for file paths.")
            }

            Button {
                enabled: action.commandLine !== ""
                // enabled: backendLocation.currentCell >= 0 && conditiong.locationCells.length < root.maxCells
                text: qsTr("Test")
                onClicked: {
                    console.log("Testing command line")
                    backendRulesModel.testCommandLine(action.commandLine)
                }
            } // Button

            SectionHeader {
                section: qsTr("When rule deactivates")
            }

            TextFieldWithLabel {
                labelText: qsTr("Command to run")
                placeholderText: qsTr("No custom action")
                text: action.commandLineExit
                // height: UIConstants.LIST_ITEM_HEIGHT_SMALL
                width: parent.width
                onTextChanged: {
                    action.commandLineExit = text
                }
            }

            LabelHelp {
                text: qsTr("Enter into the text box the shell command you want to run when this rule is deactivated. "
                      + "Always use full paths for file paths.")
            }

            Button {
                enabled: action.commandLineExit !== ""
                // enabled: backendLocation.currentCell >= 0 && conditiong.locationCells.length < root.maxCells
                text: qsTr("Test")
                onClicked: {
                    console.log("Testing command line exit")
                    backendRulesModel.testCommandLine(action.commandLineExit)
                }
            } // Button

        }
    }
}
