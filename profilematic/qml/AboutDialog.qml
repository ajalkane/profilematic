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
    titleText: "ProfileMatic v2.2.0-beta"
    message: "<b>(c) Arto Jalkanen 2011-2013</b><br>"
             + qsTr("UX design by Dina & Arto Jalkanen")
             + "<br>"
             + "<br>"
             + "<small>"
             + qsTr("Contributors:")
             + "<br>"
             + "Philip Lorenz (" + qsTr("Availability") + ")"
             + "<br>"
             + "Ferdinand Mayet (" + qsTr("Stand-by screen") + ")"
             + "<br>"
             + "<br>"
             + qsTr("Translators:")
             + "<li>Russian - Placeholder"
             + "<li>Polish - Placeholder"
             + "<li>Italian - Placeholder"
             + "<br>"
             + "<br>"
             + qsTr("See <a href=\"http://www.kolumbus.fi/~ks2877/profilematic/\">homepage</a> for more tips, sources, donation information, and more.")
             + "<br>"
             + "<br>"
             + "This application is free sofware licenced under the GNU Public License version 3"
             + "<br>"
             + "</small>"
             + "<small align=\"left\">"
             + "<p>"
             + "The time choosing dialog used in ProfileMatic is used under BSD licence:"
             + "<p>"
    + "Copyright (c) 2011-2012, Vasiliy Sorokin <sorokin.vasiliy@gmail.com>, Aleksey Mikhailichenko <a.v.mich@gmail.com>"
    + "All rights reserved."
    + "<p>"
    + "Redistribution and use in source and binary forms, with or without modification,"
    + "are permitted provided that the following conditions are met:"
    + "<br>"
    + "* Redistributions of source code must retain the above copyright notice,"
    + "this list of conditions and the following disclaimer."
    + "<br>"
    + "* Redistributions in binary form must reproduce the above copyright notice,"
    + "this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution."
    + "<br>"
    + "* Neither the name of the vsorokin nor the names of its contributors may be used to endorse or"
    + "promote products derived from this software without specific prior written permission."
    + "<p>"
    + "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND"
    + "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND"
    + "FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS"
    + "BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,"
    + "OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;"
    + "LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,"
    + "WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY"
    + "WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
    + "</small>"

    acceptButtonText: qsTr("Close")
    onLinkActivated: Qt.openUrlExternally(link)
}
