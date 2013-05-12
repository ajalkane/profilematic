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
import com.nokia.extras 1.0
import Rule 1.0
import "UIConstants.js" as UIConstants

Page {
    id: root
    tools: commonTools
    anchors.margins: UIConstants.DEFAULT_MARGIN

    property RuleCondition condition // : (rule !== null ? rule.condition : null)

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
                text: qsTr("Battery level condition")
            }

            RuleTopicSummary {
                id: summary
                topic: qsTr("Battery level")
                summary: batteryLevelSummary()
                showComboBox: true
                onTopicClicked: batteryLevelEditHandler()
            }

            Connections {
                target: condition.batteryLevel
                onChanged: {
                    console.debug("ConditionBatteryLevel onChanged")
                    summary.summary = batteryLevelSummary()
                }

            }

            Button {
                text: qsTr("Clear")
                onClicked: {
                    condition.batteryLevel.clear()
                }
            }

            LabelHelp {
                text: qsTr("This condition can be used to activate actions when battery "
                    + "level of the device is within specified range.")
            }
        }
    }

    QueryDialog {
        id: dRuleWarning

        titleText: qsTr("Invalid battery level range")
        acceptButtonText: qsTr("Ok")
        message: qsTr("Battery level minimum must be less or equal than maximum to be usable")
    }

    DialogBatteryLevel {
        id: dBatteryLevel
        onSelected: {
            console.debug("DialogBatteryLevel: min " + min + " max " + max)
            condition.batteryLevel.levelMin = min
            condition.batteryLevel.levelMax = max
            if (!condition.batteryLevel.isValid()) {
                dRuleWarning.open();
            }
        }
    }

    function batteryLevelSummary() {
        console.debug("BatteryLevelSummary called")
        var summary = backendRuleUtil.batteryLevelSummary(condition, qsTr("Not used"), false);
        return summary
    }

    function batteryLevelEditHandler() {
        console.log("ConditionBatteryLevel editHandler, initializing from " + condition.batteryLevel.levelMin + " / " + condition.batteryLevel.levelMax)
        dBatteryLevel.selectedMin = condition.batteryLevel.levelMin > 0 ? condition.batteryLevel.levelMin : 0
        dBatteryLevel.selectedMax = condition.batteryLevel.levelMax > 0 ? condition.batteryLevel.levelMax : 100
        dBatteryLevel.open();
    }
}
