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
#include <gconfitem.h>

#include "actionstandbyscreenmode.h"

ActionStandByScreenMode::ActionStandByScreenMode(PlatformUtil *platformUtil)
    : _platformUtil(platformUtil), _previousSbsmState(-1)
{

}

void
ActionStandByScreenMode::activate(const Rule &rule) {

    int standByScreenMode = rule.getStandByScreenMode();

    if ((standByScreenMode < 0 || rule.isDefaultRule()) && _previousSbsmState >= 0) {
        qDebug("ActionStandByScreenMode::activate restore, sbsmState not set or is default rule for rule %s",
               qPrintable(rule.getRuleName()));
        qDebug("ActionStandByScreenMode::activate previous rule had restore sbsmState, restoring sbsmState %d",
               _previousSbsmState);
        standByScreenMode = _previousSbsmState;
        _previousSbsmState = -1;
    }
    else if (standByScreenMode < 0) {
        qDebug("ActionStandByScreenMode::activate not setting stand-by-screen state for rule %s",
               qPrintable(rule.getRuleName()));
        return;
    }

    GConfItem entry("/system/osso/dsm/display/use_low_power_mode");

    if (rule.getStandByScreenMode() == 1) {
        entry.set(true);
        qDebug("ActionStandByScreenMode::activate StandByScreen set");
        _platformUtil->publishNotification("StandByScreen action set for " + rule.getRuleName());
    } else {
        entry.set(false);
        qDebug("ActionStandByScreenMode::activate StandByScreen unset");
        _platformUtil->publishNotification("StandByScreen action unset for " + rule.getRuleName());
    }

}
