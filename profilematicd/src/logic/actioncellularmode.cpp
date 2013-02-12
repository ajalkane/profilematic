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
#include "actioncellularmode.h"

ActionCellularMode::ActionCellularMode()
    : _previousCellularMode(-1)
{
}

bool
ActionCellularMode::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) {
    int cellularMode = rule.getCellularMode();
    IFDEBUG(qDebug("ActionCellularMode::activate: %d", cellularMode));
    bool activated = true;

    if (useRestoreAction(ruleId, cellularMode >= 0, _previousCellularMode >= 0)) {
        IFDEBUG(qDebug("ActionCellularMode::activate restore"));
        IFDEBUG(qDebug("ActionCellularMode::activate previous rule had restore cellularMode, restoring cellularMode %d",
               _previousCellularMode));
        cellularMode = _previousCellularMode;
        _previousCellularMode = -1;
        // Restore is not returned as activation
        activated = false;
    }
    else if (cellularMode < 0) {
        IFDEBUG(qDebug("ActionCellularMode::activate not setting flight mode"));
        return false;
    }

    if (rule.getRestoreCellularMode()) {
        _previousCellularMode = PlatformUtil::instance()->cellularMode();
    } else {
        _previousCellularMode = -1;
    }

    PlatformUtil::instance()->setCellularMode(cellularMode);

    return activated;
}
