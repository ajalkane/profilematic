/**********************************************************************
 * Copyright 2011 Arto Jalkanen
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

ListModel {
    function getDayIndices(index) {
        var days = eval(get(index).days);
        console.log("getDayIndices days", days)
        console.log("getDayIndices length", days.length)
        var daysArr = new Array(days.length)
        for (var i = 0; i < days.length; ++i) {
            console.log("days", i, days[i])
            daysArr[i] = days[i]
        }
        return daysArr
    }

    function setDayIndices(index, dayIndices) {
        var daysString = "["
        for (var i = 0; i < dayIndices.length; ++i) {
            if (i != 0) daysString = daysString + ", "
            daysString = daysString + dayIndices[i]
        }
        daysString = daysString + "]"
        setProperty(index, "days", daysString)
    }

    ListElement {
        ruleId: 1
        ruleName: "Rule #1"
        subtitle: "My rule #1"
        timeStartActive: true
        timeStart: "09:00"
        daysActive: true
        days: "[1, 2, 5]"
        profile: "Ringing"
        profileVolumeActive: true
        profileVolume: 40
    }
    ListElement {
        ruleId: 2
        ruleName: "Rule #2"
        subtitle: "My rule #2"
        timeStartActive: false
        timeStart: "10:00"
        daysActive: false
        days: "[1, 2, 4]"
        profile: "Ringing"
        profileVolumeActive: false
        profileVolume: 80
    }

    ListElement {
        ruleId: 3
        ruleName: "null"
        subtitle: "null"
        timeStartActive: true
        timeStart: ""
        daysActive: true
        days: "[]"
        profile: ""
        profileVolumeActive: false
        profileVolume: -1
    }

}
