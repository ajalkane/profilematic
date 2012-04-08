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

ActionPowerSavingMode::ActionPowerSavingMode()
    : _previousPsmState(-1)
{

}

bool
ActionPowerSavingMode::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) {
    int powerSavingMode = rule.getPowerSavingMode();
    bool activated = true;

    if (useRestoreAction(ruleId, powerSavingMode >= 0, _previousPsmState >= 0)) {
        qDebug("ActionPsmState::activate restore, psmState not set or is default rule");
        qDebug("ActionPsmState::activate previous rule had restore psmState, restoring psmState %d",
               _previousPsmState);
        powerSavingMode = _previousPsmState;
        _previousPsmState = -1;
        // Restore is not returned as activation
        activated = false;
    }
    else if (powerSavingMode < 0) {
        qDebug("ActionPsmState::activate not setting power saving state");
        return false;
    }
    if (rule.getRestorePowerSavingMode()) {
        _previousPsmState = PlatformUtil::instance()->powerSavingMode();
    } else {
        _previousPsmState = -1;
    }

    qDebug("Setting power saving mode %d", powerSavingMode);
    PlatformUtil::instance()->setPowerSavingMode(powerSavingMode);
    return activated;
}
