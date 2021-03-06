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

PageStackWindow {
    id: root
    QueryDialog {
        id: dQuery
        // NOTE: no need to translate, this is ancient
        titleText: "ProfileMatic update information"
        message: "ProfileMatic now activates all matching rules instead "
                 + "of only the first matching rule. In some rare cases "
                 + "this can affect how your existing rules work."
        onAccepted: Qt.quit();
        onRejected: Qt.quit();
    }

    Component.onCompleted: {
        dQuery.open();
    }
}
