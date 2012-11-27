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
#ifndef CONDITIONMANAGERCALENDAR_H
#define CONDITIONMANAGERCALENDAR_H

#include <QDateTime>
#include <QList>
#include <QTimer>

#include "../../platform/calendar/calendarentry.h"
#include "../conditionmanager.h"

#include "calendarentrymatcherdatetime.h"


class ConditionManagerCalendar : public ConditionManager
{
    Q_OBJECT

    bool _entriesLoaded;
    QList<CalendarEntry> _entries;
    QDateTime _nextNearestDateTime;
    QDateTime _refreshTime;
    QTimer _timer;

    // Contains indicies of entries that might match if the dates are in range.
    // In other words, if time
    QList<int> _potentialMatchingEntryIndices;

    void _ensureCalendarEntriesLoaded();
    void _updateNextNearestDateTime(const CalendarEntry &entry, const CalendarEntryMatcherDateTime &matcher);

private slots:
    void askRefresh();

public:
    ConditionManagerCalendar(QObject *parent = 0);

    virtual void startRefresh();
    virtual bool refresh(const Rule::IdType &, const RuleCondition &rule);
    virtual void endRefresh();

};

#endif // CONDITIONMANAGERCALENDAR_H
