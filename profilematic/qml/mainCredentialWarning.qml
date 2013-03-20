/**********************************************************************
 * Copyright 2012 Arto Jalkanen
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

PageStackWindow {
    id: root
    QueryDialog {
        id: dQuery
        titleText: qsTr("Warning")
        message: qsTr("ProfileMatic lacks credentials that are needed to "
                 + "change flight mode, and you "
                 + "have such rules."
                 + "\n\n"
                 + "You can try to reinstall ProfileMatic to restore the credentials.")
        onAccepted: Qt.quit();
        onRejected: Qt.quit();
    }

    Component.onCompleted: {
        dQuery.open();
    }
}
