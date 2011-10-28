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
    function getName(index) {
        return get(index).name
    }

    function getShortName(index) {
        return get(index).shortName
    }

    ListElement { dayId: 1; name: "Monday"; shortName: "Mon" }
    ListElement { dayId: 2; name: "Tuesday"; shortName: "Tue" }
    ListElement { dayId: 3; name: "Wednesday"; shortName: "Wed" }
    ListElement { dayId: 4; name: "Thursday"; shortName: "Thu" }
    ListElement { dayId: 5; name: "Friday"; shortName: "Fri" }
    ListElement { dayId: 6; name: "Saturday"; shortName: "Sat" }
    ListElement { dayId: 7; name: "Sunday"; shortName: "Sun" }
}
