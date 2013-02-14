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

#include "actionalarm.h"
#include "../platform/platformutil.h"

ActionAlarm::ActionAlarm()
{
}

bool
ActionAlarm::activateOnce(const Rule::IdType &, const RuleAction &action) {
    if (!action.alarm().isValid()) {
        IFDEBUG(qDebug() << "ActionAlarm::activateOnce no alarm action");
        return false;
    }

    const RuleActionAlarm &a = action.alarm();
    PlatformUtil::instance()->scheduleAlarm(a.getTitle(), a.getAlarmInSeconds());

    return true;
}

void
ActionAlarm::activateOnceExit(const Rule::IdType &, const RuleAction &action) {
    Q_UNUSED(action)
    // At least for now, nothing
}
