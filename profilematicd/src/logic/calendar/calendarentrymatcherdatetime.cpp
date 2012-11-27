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
#include "calendarentrymatcherdatetime.h"

CalendarEntryMatcherDateTime::CalendarEntryMatcherDateTime(const RuleConditionCalendar &condition, const QDateTime &now)
    : _now(now)
{
}

namespace {
    QDateTime useEndTime(const CalendarEntry &entry) {
        if (entry.start() == entry.end()) {
            // If they start and end time are the same, then set end of time as minute from start
            return entry.end().addSecs(60);
        }
        return entry.end();
    }
}
bool
CalendarEntryMatcherDateTime::match(const CalendarEntry &entry) {
    QDateTime useEnd = useEndTime(entry);
    return _now >= entry.start() && _now <= useEnd;
}

QDateTime
CalendarEntryMatcherDateTime::nextNearestStartOrEnd(const QDateTime &from, const CalendarEntry &entry) const {
    QDateTime nextNearest;
    QDateTime useEnd = useEndTime(entry);
    if (from < entry.start()) {
        qint64 msecsToEntry = from.msecsTo(entry.start());
        nextNearest = from.addMSecs(msecsToEntry);
        qDebug("CalendarEntryMatcherDateTime::next start, from %s is %s. Entry: %s",
               qPrintable(from.toString()), qPrintable(nextNearest.toString()), qPrintable(entry.summary()));
    } else if (from < useEnd) {
        qint64 msecsToEntry = from.msecsTo(useEnd);
        nextNearest = from.addMSecs(msecsToEntry);
        qDebug("CalendarEntryMatcherDateTime::next end, from %s is %s. Entry: %s",
               qPrintable(from.toString()), qPrintable(nextNearest.toString()), qPrintable(entry.summary()));
    }
    return nextNearest;
}
