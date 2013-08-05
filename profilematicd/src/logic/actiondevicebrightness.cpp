/**********************************************************************
 * Copyright 2013 Arto Jalkanen
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

#include "actiondevicebrightness.h"

#include "../platform/platformutil.h"

ActionDeviceBrightness::ActionDeviceBrightness()
    : _previousDeviceBrightness(-1)
{
}

bool
ActionDeviceBrightness::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) {
    int deviceBrightness = rule.getDeviceBrightness();
    bool activated = true;

    if (useRestoreAction(ruleId, deviceBrightness >= 0, _previousDeviceBrightness >= 0)) {
        IFDEBUG(qDebug() << Q_FUNC_INFO << "restore, deviceBrightness not set or is default rule");
        IFDEBUG(qDebug() << Q_FUNC_INFO << "previous rule had restore deviceBrightness, restoring deviceBrightness"
                         << _previousDeviceBrightness);
        deviceBrightness = _previousDeviceBrightness;
        _previousDeviceBrightness = -1;
        // Restore is not returned as activation
        activated = false;
    }
    else if (deviceBrightness < 0) {
        IFDEBUG(qDebug() << Q_FUNC_INFO << "not setting device brightness");
        return false;
    }
    if (rule.getRestoreDeviceBrightness()) {
        _previousDeviceBrightness = PlatformUtil::instance()->deviceBrightness();
    } else {
        _previousDeviceBrightness = -1;
    }

    IFDEBUG(qDebug() << Q_FUNC_INFO << "Setting" << deviceBrightness);
    PlatformUtil::instance()->setDeviceBrightness(deviceBrightness);
    return activated;
}
