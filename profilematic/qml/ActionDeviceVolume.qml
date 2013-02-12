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
                text: "Device volume action"
            }

            RuleTopicSummary {
                id: summary
                topic: "Select device volume"
                summary: deviceVolumeSummary();
                showComboBox: true
                onTopicClicked: deviceVolumeEditHandler()
            }

            Connections {
                target: action
                onDeviceVolumeChanged: {
                    console.debug("ActionDeviceVolume onChanged")
                    summary.summary = deviceVolumeSummary()
                }
            }

            Button {
                text: "Clear"
                onClicked: dConfirmDelete.open()
            }

//            Item {
//                id: restoreContainer
//                width: parent.width
//                height: action.profile !== '' ? restoreProfile.height : 0
//                clip: true

//                Behavior on height {
//                    NumberAnimation { duration: 300 }
//                }
//                RuleTopicSummary {
//                    id: restoreProfile
//                    topic: "Restore previous profile"
//                    topicHeight: Math.max(topicImplicitHeight, restoreSwitch.height)
//                    topicWidth: parent.width - restoreSwitch.width
//                    summary: restoreSwitch.checked ? "The previous profile will be restored."
//                                                   : "Previous profile will not be restored."
//                    // visible: action.profile !== ''
//                    onTopicClicked: restoreSwitch.checked = !restoreSwitch.checked
//                }
//                Switch {
//                    id: restoreSwitch
//                    checked: action.restoreProfile
//                    anchors.right: restoreProfileContainer.right // container.right
//                    anchors.top: parent.top
//                    anchors.verticalCenter: parent.top
//                    onCheckedChanged: {
//                        action.restoreProfile = checked
//                    }
//                }
//            }

        }
    }

    function deviceVolumeSummary() {
        return backendRuleUtil.deviceVolumeSummary(action, "Not used", false);
    }

    function deviceVolumeEditHandler() {
        dVolume.openWithValue(action.deviceVolume)
    }

    QueryDialog {
        id: dVolume

        titleText: "Choose volume"
        acceptButtonText: "OK"
        rejectButtonText: "Cancel"
        property alias volumeValue: volumeSlider.value

        content: Item {
            Slider {
                id: volumeSlider
                // Device volume is always for ProfileMatic in percentages. Harmattan's volume is between 0-22, so
                // that doesn't fit all that well to discreet values between 0-100. Let's just use a stepsize of 4, close enough.
                minimumValue: 0;
                maximumValue: 100;
                stepSize: 4
                valueIndicatorVisible: true
                width: dVolume.width
            }
        }

        onAccepted: {
            action.deviceVolume = volumeValue
        }

        function openWithValue(volume) {
            volumeValue = volume
            open()
        }
    }

    QueryDialog {
        id: dConfirmDelete

        titleText: "Clear device volume"
        message: "Are you sure you want to clear device volume?"
        acceptButtonText: "Yes"
        rejectButtonText: "Cancel"

        onAccepted: {
            backendRuleUtil.deviceVolumeClear(action)
        }
    }
}
