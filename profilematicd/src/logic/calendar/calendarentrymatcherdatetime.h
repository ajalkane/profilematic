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
#ifndef CALENDARENTRYMATCHERDATETIME_H
#define CALENDARENTRYMATCHERDATETIME_H

#include "calendarentrymatcher.h"

class CalendarEntryMatcherDateTime : public CalendarEntryMatcher
{
    QDateTime _now;

public:
    CalendarEntryMatcherDateTime(const RuleConditionCalendar &condition, const QDateTime &now);

    virtual bool match(const CalendarEntry &entry);

    // Returns invalid if no next nearest time for the entry, from the given time
    QDateTime nextNearestStartOrEnd(const QDateTime &from, const CalendarEntry &entry) const;
};

#endif // CALENDARENTRYMATCHERDATETIME_H
