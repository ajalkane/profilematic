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
    function getProfile(index) {
        return get(index).profile
    }

    function getProfileType(index) {
        return get(index).profileType
    }

    function getProfileToName(profile) {
        for (var i = 0; i < count; ++i) {
            if (get(i).profile == profile) {
                return get(i).name
            }
        }
        return "Unrecognized " + profile
    }

    function profileHasVolume(profile) {
        return profile == "General"
    }

    ListElement { profile: "Silent profile"; profileType: "Silent"; name: "SilentN" }
    ListElement { profile: "Meeting"; profileType: "Beep"; name: "BeepN" }
    ListElement { profile: "General"; profileType: "Ringing"; name: "RingingN" }
}
