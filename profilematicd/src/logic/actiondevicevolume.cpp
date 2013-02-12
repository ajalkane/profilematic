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
{
}

bool
ActionDeviceVolume::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) {
    Q_UNUSED(ruleId)

    int deviceVolume = rule.getDeviceVolume();

    if (deviceVolume < 0) {
        IFDEBUG(qDebug("ActionDeviceVolume::activate deviceVolume is empty, not setting"));
        return false;
    }

    IFDEBUG(qDebug() << "ActionDeviceVolume::activate deviceVolume" << deviceVolume);

    PlatformUtil::instance()->setDeviceVolume(deviceVolume);

    return true;
}
