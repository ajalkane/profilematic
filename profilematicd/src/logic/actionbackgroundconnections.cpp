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
#include "actionbackgroundconnections.h"

ActionBackgroundConnections::ActionBackgroundConnections()
    : _previousBackgroundConnectionsMode(-1)
{
}

bool
ActionBackgroundConnections::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) {
    int backgroundConnectionsMode = rule.getBackgroundConnectionsMode();
    bool activated = true;

    if (useRestoreAction(ruleId, backgroundConnectionsMode >= 0, _previousBackgroundConnectionsMode >= 0)) {
        qDebug("ActionBackgroundConnections::activate restore, backgroundConnectionsMode not set or is default rule");
        qDebug("ActionBackgroundConnections::activate previous rule had restore backgroundConnectionsMode, restoring backgroundConnectionsMode %d",
               _previousBackgroundConnectionsMode);
        backgroundConnectionsMode = _previousBackgroundConnectionsMode;
        _previousBackgroundConnectionsMode = -1;
        // Restore is not returned as activation
        activated = false;
    }
    else if (backgroundConnectionsMode < 0) {
        qDebug("ActionBackgroundConnections::activate not setting background connections mode");
        return false;
    }
    if (rule.getRestoreBackgroundConnectionsMode()) {
        _previousBackgroundConnectionsMode = PlatformUtil::instance()->backgroundConnectionsMode();
    } else {
        _previousBackgroundConnectionsMode = -1;
    }

    qDebug("Setting backgroundConnectionsMode %d", backgroundConnectionsMode);
    PlatformUtil::instance()->setBackgroundConnectionsMode(backgroundConnectionsMode);
    return activated;
}
