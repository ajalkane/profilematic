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

    property Rule    rule;

    SectionHeader {
        id: header
        section: "Set power saving mode"
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
                topic: "Select power saving mode"
                summary: powerSavingModeSummary();
                showComboBox: true
                onTopicClicked: powerSavingModeEditHandler()
            }

            Item {
                id: restoreContainer
                width: parent.width
                height: rule.powerSavingMode >= 0 ? restore.height : 0
                clip: true

                Behavior on height {
                    NumberAnimation { duration: 300 }
                }
                RuleTopicSummary {
                    id: restore
                    topic: "Restore previous power saving mode"
                    topicHeight: Math.max(topicImplicitHeight, restoreSwitch.height)
                    topicWidth: parent.width - restoreSwitch.width
                    summary: restoreSwitch.checked ? "The previous power saving mode will be restored."
                                                   : "Previous power saving mode will not be restored."
                    onTopicClicked: restoreSwitch.checked = !restoreSwitch.checked
                }
                Switch {
                    id: restoreSwitch
                    checked: rule.restorePowerSavingMode
                    anchors.right: restoreContainer.right
                    anchors.top: parent.top
                    anchors.verticalCenter: parent.top
                    onCheckedChanged: {
                        rule.restorePowerSavingMode = checked
                    }
                }
            }

            Text {
                visible: restore.visible
                wrapMode: Text.WordWrap
                width: parent.width
                font.pixelSize: UIConstants.FONT_SMALL;
                color: !theme.inverted ? UIConstants.COLOR_SECONDARY_FOREGROUND : UIConstants.COLOR_INVERTED_SECONDARY_FOREGROUND
                text: "If you activate restore previous power saving mode, ProfileMatic will set back the power saving mode setting "
                      + "that was selected before this rule was activated. If you don't activate "
                      + "restore previous power saving mode, the latest setting will continue. "
                      + "E.g. phone set to 'power saving mode on' by this rule will remain on power saving mode until you decide to make "
                      + "changes or another rule that sets power saving mode becomes valid."
                      + "\n\n"
                      + "Note: Restore power saving mode will override power saving mode setting in Default rule. E.g if you "
                      + "have Default rule to set 'power saving mode on', but deactivating current rule wants to restore "
                      + "'power saving mode off', then 'power saving mode off' will be selected."
            }

        }
    }

    PowerSavingDialog {
        id: dPowerSavingMode

        onPowerSavingModeSelected: {
            console.log("PowerSavingDialog onPowerSavingModeSelected ", selectedPowerSavingMode)
            rule.powerSavingMode = selectedPowerSavingMode
        }
    }

    function powerSavingModeSummary() {
        return dPowerSavingMode.powerSavingModeToText(rule.powerSavingMode)
    }

    function powerSavingModeEditHandler() {
        dPowerSavingMode.selectedPowerSavingMode = rule.powerSavingMode
        dPowerSavingMode.open();
    }
}
