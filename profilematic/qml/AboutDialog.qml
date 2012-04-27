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
import com.nokia.meego 1.0

MyQueryDialog {
    titleText: "ProfileMatic v1.6.1"
    message: "<b>(c) Arto Jalkanen 2011-2012</b><br>"
             + "UX design by Dina & Arto Jalkanen<br>"
             + "<br>"
             + "Contributors:"
             + "<br>"
             + "Philip Lorenz (Availability)"
             + "<br>"
             + "Ferdinand Mayet (Stand-by screen)"
             + "<br>"
             + "<br>"
             + "See <a href=\"http://www.kolumbus.fi/~ks2877/profilematic/\">homepage</a> for more tips, sources, donation information, and more."
             + "<br>"
             + "<br>"
             + "<small>This application is free sofware licenced under the GNU Public License version 3</small>"
             + "<br>"
    acceptButtonText: "Close"
    onLinkActivated: Qt.openUrlExternally(link)
}
