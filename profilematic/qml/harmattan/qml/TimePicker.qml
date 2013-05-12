import QtQuick 1.1
import QtMobility.feedback 1.1

/*
Copyright (c) 2011-2012, Vasiliy Sorokin <sorokin.vasiliy@gmail.com>, Aleksey Mikhailichenko <a.v.mich@gmail.com>, Arto Jalkanen <ajalkane@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of the vsorokin nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


Usage:

    TimePicker {
        id: timePicker
        anchors.centerIn: parent

        function orientationSuffix() {
            if (screen.currentOrientation === Screen.Portrait || screen.currentOrientation === Screen.PortraitInverted )
                return "portrait"
            else
                return "landscape"
        }

        backgroundImage: "image://theme/meegotouch-timepicker-light-1-" + orientationSuffix()
        hourDotImage: "image://theme/meegotouch-timepicker-disc-hours-" + orientationSuffix()
        minutesDotImage: "image://theme/meegotouch-timepicker-disc-minutes-" + orientationSuffix()
    }

If you want to use TimePicker within a Flickable, you will need to disable flickable while TimePicker is active. Example:

    Flickable {
        id: flickable
        anchors.fill: parent
        pressDelay: 0 // No delay wanted when hitting TimePicker's area
        clip: true
        contentWidth: parent.width
        contentHeight: childrenRect.height

        TimePicker {
            id: timePicker
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
*/


Item {
    id: timePicker

    property int hours: 0
    property int minutes: 0

    property alias backgroundImage: bg.source
    property alias hourDotImage: hourDot.source
    property alias minutesDotImage: minuteDot.source

    property int minuteGradDelta: 6
    property int hourGradDelta: 30

    signal startInteraction
    signal stopInteraction

    width: bg.sourceSize.width
    height: bg.sourceSize.height

    onHoursChanged: {
        if (hours == 24)
            hours = 0
    }

    onMinutesChanged: {
        if (minutes == 60)
            minutes = 0
    }

    Image {
        id: bg

        anchors.fill: parent

        property int centerX: parent.width / 2
        property int centerY: parent.height / 2

        Image {
            id: hourDot
            anchors.fill: parent
            rotation: timePicker.hours * 30
            smooth: true
        }

        Text {
            id: hourText
            property int hourRadius: parent.width * 0.055
            property int hourTrackRadius: parent.width * 0.16

            x: (parent.centerX - hourRadius) + hourTrackRadius
               * Math.cos(timePicker.hours * timePicker.hourGradDelta * (Math.PI / 180) - (Math.PI / 2));
            y: (parent.centerY - hourRadius) + hourTrackRadius
               * Math.sin(timePicker.hours * timePicker.hourGradDelta * (Math.PI / 180) - (Math.PI / 2));

            font.pixelSize: timePicker.width * 0.1

            text: (timePicker.hours < 10 ? "0" : "") + timePicker.hours
        }

        Image {
            id: minuteDot
            anchors.fill: parent
            rotation: timePicker.minutes * 6
            smooth: true
        }

        Text {
            id: minuteText
            property int minuteRadius: parent.width * 0.055
            property int minuteTrackRadius: parent.width * 0.38

            x: parent.centerX - minuteRadius + minuteTrackRadius
                * Math.cos(timePicker.minutes * timePicker.minuteGradDelta * (Math.PI / 180) - (Math.PI / 2));
            y: parent.centerY - minuteRadius + minuteTrackRadius
                * Math.sin(timePicker.minutes * timePicker.minuteGradDelta * (Math.PI / 180) - (Math.PI / 2));

            font.pixelSize: timePicker.width * 0.1
            color: "#CCCCCC"
            text: (timePicker.minutes < 10 ? "0" : "") + timePicker.minutes
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        property int currentHandler : -1 // 0 - hours, 1 - minutes
        property real previousAlpha: -1

        onPressed: {
            startInteraction()
            currentHandler = chooseHandler(mouseX, mouseY)
            previousAlpha = findAlpha(mouseX, mouseY)
        }

        onReleased: {
            stopInteraction()
            currentHandler = -1
            previousAlpha = -1
        }

        onPositionChanged: {
            var newAlpha = 0;
            if (currentHandler < 0)
                return

            newAlpha = findAlpha(mouseX, mouseY)

            var doRumble = false
            if (currentHandler > 0) {
                var newMins = getNewTime(timePicker.minutes, newAlpha, timePicker.minuteGradDelta, 1)
                if (newMins !== timePicker.minutes) {
                    timePicker.minutes = newMins
                    doRumble = true
                }
            } else {
                var newHours = getNewTime(timePicker.hours, newAlpha, timePicker.hourGradDelta, 2)
                if (newHours !== timePicker.hours) {
                    timePicker.hours = newHours
                    doRumble = true
                }

            }
            if (doRumble) {
                console.log("TimePicker rumble start")
                rumbleEffect.start()
            }
        }

        function sign(number) {
            return  number >= 0 ? 1 : -1;
        }

        function getNewTime(source, alpha, resolution, boundFactor) {
            var delta = alpha - previousAlpha

            if (Math.abs(delta) < resolution)
                return source

            if (Math.abs(delta) > 180) {
                delta = delta - sign(delta) * 360
            }

            var result = source * resolution

            var resdel = Math.round(result + delta)
            if (Math.round(result + delta) > 359 * boundFactor)
                result += delta - 360 * (source * resolution > 359 ? boundFactor : 1)
            else if (Math.round(result + delta) < 0 * boundFactor)
                result += delta + 360 * (source * resolution > 359 ? boundFactor : boundFactor)
            else
                result += delta

            previousAlpha = alpha
            return result / resolution
        }

        function findAlpha(x, y) {

            var alpha = (Math.atan((y - bg.centerY)/(x - bg.centerX)) * 180) / 3.14 + 90
            if (x < bg.centerX)
                alpha += 180

            return alpha
        }

        function chooseHandler(mouseX, mouseY) {
            var radius = Math.sqrt(Math.pow(bg.centerX - mouseX, 2) + Math.pow(bg.centerY - mouseY, 2));
            if (radius <= bg.width * 0.25)
                return 0
            else if(radius < bg.width * 0.5)
                return 1
            return -1
        }

    }

    HapticsEffect {
         id: rumbleEffect
         intensity: 0.1
         duration: 1
     }
}
