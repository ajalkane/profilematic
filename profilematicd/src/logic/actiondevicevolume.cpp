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
#include <QDebug>

#include "actiondevicevolume.h"
#include "../platform/platformutil.h"

ActionDeviceVolume::ActionDeviceVolume()
    : _previousDeviceVolume(-1)
{
}

bool
ActionDeviceVolume::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) {
    int deviceVolume = rule.getDeviceVolume();
    bool activated = true;

    if (useRestoreAction(ruleId, deviceVolume >= 0, _previousDeviceVolume >= 0)) {
        IFDEBUG(qDebug() << Q_FUNC_INFO << "restore, deviceVolume not set or is default rule");
        IFDEBUG(qDebug() << Q_FUNC_INFO << "previous rule had restore deviceVolume, restoring deviceVolume"
                         << _previousDeviceVolume);
        deviceVolume = _previousDeviceVolume;
        _previousDeviceVolume = -1;
        // Restore is not returned as activation
        activated = false;
    }
    else if (deviceVolume < 0) {
        IFDEBUG(qDebug() << Q_FUNC_INFO << "not setting device volume");
        return false;
    }
    if (rule.getRestoreDeviceVolume()) {
        _previousDeviceVolume = PlatformUtil::instance()->deviceVolume();
    } else {
        _previousDeviceVolume= -1;
    }

    IFDEBUG(qDebug() << Q_FUNC_INFO << "Setting" << deviceVolume);
    PlatformUtil::instance()->setDeviceVolume(deviceVolume);
    return activated;
}
