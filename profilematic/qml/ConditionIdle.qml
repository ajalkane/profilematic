/**********************************************************************
 * Copyright 2012 Arto Jalkanen
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
    property int maxCells: 50

    Flickable {
        anchors.fill: parent
        // anchors.topMargin: header.height
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

            TextFieldWithLabel {
                labelText: "Idle in minutes"
                placeholderText: "Not set"
                text: condition.idleForSecs < 0 ? "" : condition.idleForSecs / 60
                inputMethodHints: Qt.ImhDigitsOnly
                validator: RegExpValidator { regExp: /\d{0,4}/ }
                width: parent.width
                onTextChanged: {
                    if (root.status === PageStatus.Active) {
                        condition.idleForSecs = (text !== "" ? parseInt(text) * 60 : -1)
                    }
                }
            }

            LabelHelp {
                text: "Specify the amount of time in minutes that the device has to be in idle mode until this "
                      + "rule is activated. Device goes into idle if user does not do anything for a while (about 30 seconds). "
                      + "Idle mode is exited when lock screen is deactivated."
            }

        } // Column
    } // Flickable
}
