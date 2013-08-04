/**********************************************************************
 * Copyright 2011-2013 Arto Jalkanen
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
                text: qsTr("Device brightness action")
            }

            RuleTopicSummary {
                id: summary
                topic: qsTr("Select device brightness")
                summary: deviceBrightnessSummary();
                showComboBox: true
                onTopicClicked: deviceBrightnessEditHandler()
            }

            Connections {
                target: action
                onDeviceBrightnessChanged: {
                    console.debug("ActionBrightness onChanged")
                    summary.summary = deviceBrightnessSummary()
                }
            }

            Button {
                text: qsTr("Clear")
                onClicked: dConfirmDelete.open()
            }

            Item {
                id: restoreContainer
                width: parent.width
                height: action.deviceBrightness > 0 ? restoreValue.height : 0
                clip: true

                Behavior on height {
                    NumberAnimation { duration: 300 }
                }
                RuleTopicSummary {
                    id: restoreValue
                    topic: "Restore previous brightness"
                    topicHeight: Math.max(topicImplicitHeight, restoreSwitch.height)
                    topicWidth: parent.width - restoreSwitch.width
                    summary: restoreSwitch.checked ? "The previous brightness will be restored."
                                                   : "Previous brightness will not be restored."
                    // visible: action.profile !== ''
                    onTopicClicked: restoreSwitch.checked = !restoreSwitch.checked
                }
                Switch {
                    id: restoreSwitch
                    checked: action.restoreDeviceBrightness
                    anchors.right: restoreContainer.right // container.right
                    anchors.top: parent.top
                    anchors.verticalCenter: parent.top
                    onCheckedChanged: {
                        action.restoreDeviceBrightness = checked
                    }
                }
            }

        }
    }

    function deviceBrightnessSummary() {
        return backendRuleUtil.deviceBrightnessSummary(action, qsTr("Not used"), false);
    }

    function deviceBrightnessEditHandler() {
        dBrightness.openWithValue(action.deviceBrightness)
    }

    QueryDialog {
        id: dBrightness

        titleText: qsTr("Choose brightness")
        acceptButtonText: qsTr("OK")
        rejectButtonText: qsTr("Cancel")
        property alias brightnessValue: slider.value

        content: Item {
            Slider {
                id: slider
                // Device brightness is always for ProfileMatic in percentages. Harmattan's brightness is between 1-5, so
                // step size is 100 / ((5 - 1) + 1) = 20
                minimumValue: 0;
                maximumValue: 100;
                stepSize: 20
                valueIndicatorVisible: true
                width: dBrightness.width
            }
        }

        onAccepted: {
            action.deviceBrightness = brightnessValue
        }

        function openWithValue(value) {
            brightnessValue = value
            open()
        }
    }

    QueryDialog {
        id: dConfirmDelete

        titleText: qsTr("Clear device brightness")
        message: qsTr("Are you sure you want to clear device brightness?")
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("Cancel")

        onAccepted: {
            backendRuleUtil.deviceBrightnessClear(action)
        }
    }
}
