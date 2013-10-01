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
#include "../../util/conditionallogging.h"
#include "calendarentrymatcherdatetime.h"

CalendarEntryMatcherDateTime::CalendarEntryMatcherDateTime(const RuleConditionCalendar &condition, const QDateTime &now)
    : _now(now), _prepend(condition.getTimePrepend()), _append(condition.getTimeAppend())
{
}

QDateTime
CalendarEntryMatcherDateTime::_useEndTime(const CalendarEntry &entry) const {
    QDateTime endTime = entry.end().addSecs(_append);

    if (entry.start() == entry.end()) {
        // If they start and end time are the same, then set end of time as minute _now start
        return endTime.addSecs(60);
    }
    return endTime;
}

QDateTime
CalendarEntryMatcherDateTime::_useStartTime(const CalendarEntry &entry) const {
    QDateTime startTime = entry.start().addSecs(-_prepend);
    return startTime;
}

bool
CalendarEntryMatcherDateTime::match(const CalendarEntry &entry) {
    QDateTime useEnd = _useEndTime(entry);
    QDateTime useStart = _useStartTime(entry);
    return _now >= useStart && _now <= useEnd;
}

QDateTime
CalendarEntryMatcherDateTime::nextNearestStartOrEnd(const CalendarEntry &entry) const {
    QDateTime nextNearest;
    QDateTime useEnd = _useEndTime(entry);
    QDateTime useStart = _useStartTime(entry);
    if (_now < useStart) {
        qint64 msecsToEntry = _now.msecsTo(useStart);
        nextNearest = _now.addMSecs(msecsToEntry);
        IFDEBUG(qDebug("CalendarEntryMatcherDateTime::next start, _now %s is %s. Entry: %s",
               qPrintable(_now.toString()), qPrintable(nextNearest.toString()), qPrintable(entry.summary())));
    } else if (_now < useEnd) {
        qint64 msecsToEntry = _now.msecsTo(useEnd);
        nextNearest = _now.addMSecs(msecsToEntry);
        IFDEBUG(qDebug("CalendarEntryMatcherDateTime::next end, _now %s is %s. Entry: %s",
               qPrintable(_now.toString()), qPrintable(nextNearest.toString()), qPrintable(entry.summary())));
    }
    return nextNearest;
}
