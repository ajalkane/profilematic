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
#include "actionflightmode.h"

ActionFlightMode::ActionFlightMode()
    : _previousFlightMode(-1)
{
}

bool
ActionFlightMode::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) {
    int flightMode = rule.getFlightMode();
    bool activated = true;

    if (useRestoreAction(ruleId, flightMode >= 0, _previousFlightMode >= 0)) {
        IFDEBUG(qDebug("ActionFlightMode::activate restore, flightMode not set or is default rule"));
        IFDEBUG(qDebug("ActionFlightMode::activate previous rule had restore flightMode, restoring flightMode %d",
               _previousFlightMode));
        flightMode = _previousFlightMode;
        _previousFlightMode = -1;
        // Restore is not returned as activation
        activated = false;
    }
    else if (flightMode < 0) {
        IFDEBUG(qDebug("ActionFlightMode::activate not setting flight mode"));
        return false;
    }
    if (rule.getRestoreFlightMode()) {
        _previousFlightMode = PlatformUtil::instance()->flightMode();
    } else {
        _previousFlightMode = -1;
    }

    IFDEBUG(qDebug("Setting flight mode %d", flightMode));
    PlatformUtil::instance()->setFlightMode(flightMode);
    return activated;
}
