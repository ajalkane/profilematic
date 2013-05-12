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
                text: qsTr("Profile action")
            }

            RuleTopicSummary {
                topic: qsTr("Select profile")
                summary: profileSummary();
                showComboBox: true
                onTopicClicked: profileEditHandler()
            }

            Item {
                id: restoreProfileContainer
                width: parent.width
                height: action.profile !== '' ? restoreProfile.height : 0
                clip: true

                Behavior on height {
                    NumberAnimation { duration: 300 }
                }
                RuleTopicSummary {
                    id: restoreProfile
                    topic: qsTr("Restore previous profile")
                    topicHeight: Math.max(topicImplicitHeight, restoreSwitch.height)
                    topicWidth: parent.width - restoreSwitch.width
                    summary: restoreSwitch.checked ? qsTr("The previous profile will be restored.")
                                                   : qsTr("Previous profile will not be restored.")
                    // visible: action.profile !== ''
                    onTopicClicked: restoreSwitch.checked = !restoreSwitch.checked
                }
                Switch {
                    id: restoreSwitch
                    checked: action.restoreProfile
                    anchors.right: restoreProfileContainer.right // container.right
                    anchors.top: parent.top
                    anchors.verticalCenter: parent.top
                    onCheckedChanged: {
                        action.restoreProfile = checked
                    }
                }
            }

            Item {
                width: parent.width
                height: volume.visible ? volume.height : 0
                clip: true

                Behavior on height {
                    NumberAnimation { duration: 300 }
                }

                RuleTopicSummary {
                    id: volume
                    topic: qsTr("Set ringing volume")
                    summary: volumeSummary();
                    showComboBox: true
                    visible: isVolumeVisible();
                    onTopicClicked: volumeEditHandler()
                }
            }

            Text {
                visible: restoreProfile.visible
                wrapMode: Text.WordWrap
                width: parent.width
                font.pixelSize: UIConstants.FONT_SMALL;
                color: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                text: qsTr("If you activate restore previous profile, ProfileMatic will set back the profile "
                      + "that was selected before this rule was activated. If you don't activate "
                      + "restore previous profile, the latest profile will run. "
                      + "E.g. phone set to silent by this rule will remain silent until you decide to make "
                      + "changes or another rule that sets profile becomes valid.")
            }

        }
    }

    // Profile functions
    MySelectionDialog {
        id: profilesDialog
        titleText: qsTr("Set profile")
        platformStyle: SelectionDialogStyle {
            itemSelectedBackgroundColor: UIConstants.COLOR_SELECT
        }
        model: backendProfilesModel

        onSelectedIndexChanged: {
            if (selectedIndex > -1) {
                var selectedProfile = model.getProfile(selectedIndex)
                action.profile = selectedProfile
            }
        }


        function openWithSelection(selectedProfile) {
            for (var i = 0; i < backendProfilesModel.count; i++) {
                var profile = backendProfilesModel.getProfile(i)
                if (selectedProfile === profile) {
                    selectedIndex = i
                }
            }
            open()
        }
    }

    function profileSummary() {
        return action.profile !== "" ? backendProfilesModel.getProfileToName(action.profile) : qsTr("Click to set")
    }

    function profileEditHandler() {
        profilesDialog.openWithSelection(action.profile)
    }

    QueryDialog {
        id: dVolume

        titleText: qsTr("Choose ringing volume")
        acceptButtonText: qsTr("OK")
        rejectButtonText: qsTr("Cancel")
        property alias volumeValue: volumeSlider.value

        content: Item {
            Slider {
                id: volumeSlider
                // IMPROVE it might be cleaner to ask these from backend instead of hard-coding here
                minimumValue: 40;
                maximumValue: 100;
                stepSize: 20
                valueIndicatorVisible: false
                width: dVolume.width
            }
        }

        onAccepted: {
            action.profileVolume = volumeValue
        }

        function openWithValue(volume) {
            volumeValue = volume
            open()
        }
    }

    // Profile volume functions
    function volumeSummary() {
        if (action.profileVolume < 0) {
            return qsTr("Volume has not been selected yet")
        }

        return action.profileVolume + " %"
    }

    function volumeEditHandler() {
        dVolume.openWithValue(action.profileVolume)
    }

    function isVolumeVisible() {
        return backendProfilesModel.profileHasVolume(action.profile);
    }
}
