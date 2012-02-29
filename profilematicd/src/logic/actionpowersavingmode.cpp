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
#include "actionpowersavingmode.h"

ActionPowerSavingMode::ActionPowerSavingMode(PlatformUtil *platformUtil)
    : _platformUtil(platformUtil), _previousPsmState(-1)
{

}

void
ActionPowerSavingMode::activate(const Rule &rule) {
    int powerSavingMode = rule.getPowerSavingMode();

    if ((powerSavingMode < 0 || rule.isDefaultRule()) && _previousPsmState >= 0) {
        qDebug("ActionPsmState::activate restore, psmState not set or is default rule for rule %s",
               qPrintable(rule.getRuleName()));
        qDebug("ActionPsmState::activate previous rule had restore psmState, restoring psmState %d",
               _previousPsmState);
        powerSavingMode = _previousPsmState;
        _previousPsmState = -1;
    }
    else if (powerSavingMode < 0) {
        qDebug("ActionPsmState::activate not setting power saving state for rule %s",
               qPrintable(rule.getRuleName()));
        return;
    }
    if (rule.getRestorePowerSavingMode()) {
        _previousPsmState = _platformUtil->powerSavingMode();
    } else {
        _previousPsmState = -1;
    }

    qDebug("Setting power saving mode %d", powerSavingMode);
    _platformUtil->setPowerSavingMode(powerSavingMode);
}
