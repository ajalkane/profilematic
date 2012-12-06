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

#include "../../platform/calendar/calendarmanager.h"
#include "../../platform/calendar/calendarentry.h"
#include "../conditionmanagercacheable.h"

#include "calendarentrymatcherdatetime.h"


class ConditionManagerCalendar : public ConditionManagerCacheable
{
    Q_OBJECT

    CalendarManager *_calendarManager;
    QDate _entriesLoadedForDay;
    QList<CalendarEntry> _entries;
    QDateTime _nextNearestDateTime;
    QTimer _timerForNextNearest;

    // Contains indicies of entries that might match if the dates are in range.
    // In other words, if time comes into range
    QList<int> _potentialMatchingEntryIndices;

    inline bool _conditionSetForMatching(const RuleConditionCalendar &condCal) const { return condCal.isValid(); }

    void _ensureCalendarEntriesLoaded(const QDateTime &now);
    void _updateNextNearestDateTime(const CalendarEntry &entry, const CalendarEntryMatcherDateTime &matcher);
    void _startNextNearestTimer(const QDateTime &now);

private slots:
    void onCalendarChanged();
public slots:
    // Public for Unit Testing
    void onTimeoutForNextMatchCheck();
public:
    ConditionManagerCalendar(QObject *parent = 0);

    virtual bool conditionSetForMatching(const RuleCondition &cond) const;
    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond);

    virtual void startMonitor();
    virtual void stopMonitor();

    virtual void rulesChanged();

    // These functions only needed for unit tests
    MatchStatus match(const QDateTime &now, const RuleConditionCalendar &condCal);

    const QTimer *timer() const {
        return &_timerForNextNearest;
    }
    // Minimum interval in msecs
    inline int minimumIntervalSec() const {
        return _timerForNextNearest.interval() / 1000;
    }
    // Maximum interval in msecs
    inline int maximumIntervalSec() const {
        return _timerForNextNearest.interval() / 1000;
    }
    inline void setTimerMaxIntervalAddition(int add) {
        Q_UNUSED(add)
        // _timerIntervalMaxAddition = add;
    }
    inline void timerStart(int secs) {
        _timerForNextNearest.start(secs * 1000);
    }
};

#endif // CONDITIONMANAGERCALENDAR_H
