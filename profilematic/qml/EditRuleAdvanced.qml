/*********************************************************************
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
import com.nokia.extras 1.0

import Rule 1.0
import profilematic 1.0

import "UIConstants.js" as UIConstants

Page {
    id: root
    tools: commonTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property Rule    rule;

    Flickable {
        anchors.fill: parent
        pressDelay: 140
        clip: true
        contentWidth: parent.width
        contentHeight: container.height // contentItem.childrenRect.height

        Column {
            id: container
            spacing: UIConstants.DEFAULT_MARGIN
            anchors.verticalCenter: parent.verticalCenter

            width: parent.width
            height: childrenRect.height

            PageHeader {
                id: header
                text: "Rule advanced options"
            }

            RuleTopicSummarySwitch {
                topic: "Stop rule processing if matches"
                summary: checked ? "No more rules processed if this rule matches."
                                 : "Normal rule matching."
                checked: rule.stopIfMatched
                onCheckedChanged: {
                    rule.stopIfMatched = checked;
                }
            }

            LabelHelp {
                text: "ProfileMatic processes rules from top to bottom. Normally all rules are processed. If this "
                      + "option is set on, whenever this rules matches, the following rules are not processed anymore. "
                      + "Default rule is a special case, which is always processed even if this option is set on."
            }

        } // Column
    } // Flickable

}
