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
import QtQuick 1.0
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import "UIConstants.js" as UIConstants

//TimePickerDialog {
//    fields: DateTime.Hours | DateTime.Minutes
//    hourMode: DateTime.TwentyFourHours
//    acceptButtonText: "Ok"
//    rejectButtonText: "Cancel"
//}

Sheet {
    id: root
    acceptButtonText: qsTr("Ok")
    rejectButtonText: qsTr("Cancel")

    property alias hour: timePicker.hours
    property alias minute: timePicker.minutes

    property string titleText

    content: Flickable {
        id: flickable
        anchors.fill: parent
        pressDelay: 0 // No delay wanted when hitting TimePicker's area
        clip: true
        contentWidth: parent.width
        contentHeight: childrenRect.height

        Text {
            id: header
            text: root.titleText + " " + root.formatTime(timePicker.hours, timePicker.minutes)

            anchors.rightMargin: 8 // IMPROVE UIConstants
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: UIConstants.FONT_XLARGE
            font.weight: Font.Light

            color: theme.inverted ? UIConstants.COLOR_INVERTED_FOREGROUND : UIConstants.COLOR_FOREGROUND
        }

        TimePicker {
            id: timePicker
            anchors.top: header.bottom
            anchors.topMargin: UIConstants.PADDING_XXLARGE

            anchors.horizontalCenter: parent.horizontalCenter
            function orientationSuffix() {
                if (screen.currentOrientation === Screen.Portrait || screen.currentOrientation === Screen.PortraitInverted )
                    return "portrait"
                else
                    return "landscape"
            }

            backgroundImage: "image://theme/meegotouch-timepicker-light-1-" + orientationSuffix()
            hourDotImage: "image://theme/meegotouch-timepicker-disc-hours-" + orientationSuffix()
            minutesDotImage: "image://theme/meegotouch-timepicker-disc-minutes-" + orientationSuffix()

            onStartInteraction: flickable.interactive = false
            onStopInteraction: flickable.interactive = true
        }
    }
    function formatTime(hour, minute) {
        return (hour < 10 ? "0" : "") + hour + ":" + (minute < 10 ? "0" : "") + minute
    }

}
