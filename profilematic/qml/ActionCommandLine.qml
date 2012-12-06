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
                text: "Custom action"
            }

            LabelHelp {
                text: "Use this action only if you know what you're doing (ie. you're familiar with using N9's developer mode and terminal)! "
            }

            TextFieldWithLabel {
                labelText: "Custom action on rule activate"
                placeholderText: "No custom action"
                text: action.commandLine
                // height: UIConstants.LIST_ITEM_HEIGHT_SMALL
                width: parent.width
                onTextChanged: {
                    action.commandLine = text
                }
            }

            LabelHelp {
                text: "Enter into the text box the shell command you want to run when this rule is activated. "
                      + "Always use full paths for file paths."
            }

            Button {
                enabled: action.commandLine !== ""
                // enabled: backendLocation.currentCell >= 0 && conditiong.locationCells.length < root.maxCells
                text: "Test"
                onClicked: {
                    console.log("Testing command line")
                    backendRulesModel.testCommandLine(action.commandLine)
                }
            } // Button

            TextFieldWithLabel {
                labelText: "Custom action on rule deactivate"
                placeholderText: "No custom action"
                text: action.commandLineExit
                // height: UIConstants.LIST_ITEM_HEIGHT_SMALL
                width: parent.width
                onTextChanged: {
                    action.commandLineExit = text
                }
            }

            LabelHelp {
                text: "Enter into the text box the shell command you want to run when this rule is deactivated. "
                      + "Always use full paths for file paths."
            }

            Button {
                enabled: action.commandLineExit !== ""
                // enabled: backendLocation.currentCell >= 0 && conditiong.locationCells.length < root.maxCells
                text: "Test"
                onClicked: {
                    console.log("Testing command line exit")
                    backendRulesModel.testCommandLine(action.commandLineExit)
                }
            } // Button

        }
    }
}
