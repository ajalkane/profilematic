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

    property RuleCondition condition // : (rule !== null ? rule.condition : null)

    SectionHeader {
        id: header
        section: "Choose internet connection"
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

            RuleTopicSummary {
                topic: "Internet connection mode"
                summary: internetConnectionModeSummary()
                showComboBox: true
                onTopicClicked: internetConnectionModeEditHandler()
            }

            LabelHelp {
                text: ""
            }
        }
    }

    DialogInternetConnectionMode {
        id: dInternetConnectionMode

        onInternetConnectionModeSelected: {
            condition.internetConnectionMode = selectedInternetConnectionMode
        }
    }

    function internetConnectionModeSummary() {
        var summary = dInternetConnectionMode.internetConnectionModeSummary(condition.internetConnectionMode)
        return summary
    }

    function internetConnectionModeEditHandler() {
        dInternetConnectionMode.selectedInternetConnectionMode = condition.internetConnectionMode
        dInternetConnectionMode.open();
    }
}
