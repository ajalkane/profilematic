/**********************************************************************
 * Copyright 2012 Ferdinand Mayet
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

#include "actionstandbyscreenmode.h"

ActionStandByScreenMode::ActionStandByScreenMode()
    : _previousSbsmState(-1)
{

}

bool
ActionStandByScreenMode::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) {

    int standByScreenMode = rule.getStandByScreenMode();
    bool activated = true;

    if (useRestoreAction(ruleId, standByScreenMode >= 0, _previousSbsmState >= 0)) {
        IFDEBUG(qDebug("ActionStandByScreenMode::activate restore, sbsmState not set or is default rule"));
        IFDEBUG(qDebug("ActionStandByScreenMode::activate previous rule had restore sbsmState, restoring sbsmState %d",
               _previousSbsmState));
        standByScreenMode = _previousSbsmState;
        PlatformUtil::instance()->setStandByScreenMode(_previousSbsmState);
        _previousSbsmState = -1;
        // Restore is not returned as activation
        activated = false;
    }
    else if (standByScreenMode < 0) {
        IFDEBUG(qDebug("ActionStandByScreenMode::activate not setting stand-by-screen state"));
        return false;
    }

    if (standByScreenMode == 1) {
        _previousSbsmState = PlatformUtil::instance()->standByScreenMode();
        PlatformUtil::instance()->setStandByScreenMode(1);
        IFDEBUG(qDebug("ActionStandByScreenMode::activate StandByScreen enabled"));
        PlatformUtil::instance()->publishNotification("StandByScreen action enabled");
    } else {
        _previousSbsmState = PlatformUtil::instance()->standByScreenMode();
        PlatformUtil::instance()->setStandByScreenMode(0);
        IFDEBUG(qDebug("ActionStandByScreenMode::activate StandByScreen disabled"));
        PlatformUtil::instance()->publishNotification("StandByScreen action disabled");
    }

    return activated;
}
