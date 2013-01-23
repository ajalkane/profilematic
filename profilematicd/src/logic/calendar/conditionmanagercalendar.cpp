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
#include <QDebug>

#include "../../platform/platformutil.h"

#include "conditionmanagercalendar.h"
#include "calendarentrymatchercondition.h"

// Since DateTime resolution is in milliseconds, and here calculations are done
// on second resolution, a buffer of one second to get rid of errors due to
// rounding.
// Now has buffer of 2, since at least on Harmattan the timer seemed to have triggered
// between day intervals on 23:59:59. Will see if this corrects it.
#define SECS_TO_BUFFER 2

ConditionManagerCalendar::ConditionManagerCalendar(QObject *parent)
    : ConditionManagerCacheable(parent)
{
    setObjectName("ConditionManagerCalendar");

    IFDEBUG(qDebug() << "ConditionManagerCalendar::constructor");
    _timerForNextNearest.setSingleShot(true);
    _calendarManager = PlatformUtil::instance()->createCalendarManager();

    connect(&_timerForNextNearest, SIGNAL(timeout()), this, SLOT(onTimeoutForNextMatchCheck()));
    connect(_calendarManager, SIGNAL(onCalendarChanged()), this, SLOT(onCalendarChanged()));
}

bool
ConditionManagerCalendar::conditionSetForMatching(const RuleCondition &cond) const {
    return _conditionSetForMatching(cond.calendar());
}

ConditionManagerCacheable::MatchStatus
ConditionManagerCalendar::match(const Rule::IdType &, const RuleCondition &condition) {
    const RuleConditionCalendar &condCal = condition.calendar();

    if (!_conditionSetForMatching(condCal)) {
        IFDEBUG(qDebug() << "ConditionManagerCalendar::match() calendar options not set or invalid, matches");
        return MatchNotSet;
    }

    QDateTime now = QDateTime::currentDateTime();
    return match(now, condCal);
}

ConditionManagerCacheable::MatchStatus
ConditionManagerCalendar::match(const QDateTime &now, const RuleConditionCalendar &condCal) {
    _ensureCalendarEntriesLoaded(now);

    MatchStatus match = NotMatched;
    CalendarEntryMatcherCondition matcherCondition(condCal);
    CalendarEntryMatcherDateTime matcherDateTime(condCal, now);

    if (_nextNearestDateTime <= now) {
        QDate startDate = now.date();
        QDate endDate = startDate.addDays(1);

        // Initializing next nearest invalidation timer so that calendar gets reloaded
        _nextNearestDateTime = QDateTime(endDate);
        _startNextNearestTimer(now);
    }

    foreach (CalendarEntry entry, _entries) {
        if (matcherCondition.match(entry)) {
            IFDEBUG(qDebug() << "ConditionManagerCalendar::match() condition matches entry" << entry.summary());
            _updateNextNearestDateTime(entry, matcherDateTime);
            if (matcherDateTime.match(entry)) {
                match = Matched;
            }
        }
    }

    IFDEBUG(qDebug() << "ConditionManagerCalendar::match() returning" << (int)match);

    return match;
}

void
ConditionManagerCalendar::startMonitor() {
}

void
ConditionManagerCalendar::stopMonitor() {
    IFDEBUG(qDebug() << QDateTime::currentDateTime().toString() << "ConditionManagerCalendar::stopMonitor");
    _timerForNextNearest.stop();
    _nextNearestDateTime = QDateTime();
    _entries.clear();
    _entriesLoadedForDay = QDate();

    _calendarManager->closeCalendar();
}

void
ConditionManagerCalendar::rulesChanged() {
    _timerForNextNearest.stop();
    _nextNearestDateTime = QDateTime();
    _entries.clear();
    _entriesLoadedForDay = QDate();
}

void
ConditionManagerCalendar::onCalendarChanged() {
    IFDEBUG(qDebug() << QDateTime::currentDateTime().toString() << "ConditionManagerCalendar::onCalendarChanged");
    _timerForNextNearest.stop();
    _nextNearestDateTime = QDateTime();
    _entries.clear();
    _entriesLoadedForDay = QDate();
    emit matchInvalidated();
}

void
ConditionManagerCalendar::onTimeoutForNextMatchCheck() {
    IFDEBUG(qDebug() << QDateTime::currentDateTime().toString() << "ConditionManagerCalendar::onTimeoutForNextMatchCheck");
    emit matchInvalidated();
}

void
ConditionManagerCalendar::_startNextNearestTimer(const QDateTime &now) {
    int interval = now.secsTo(_nextNearestDateTime);
    interval += SECS_TO_BUFFER;
    IFDEBUG(qDebug() << "ConditionManagerCalendar: Now" << now.toString());
    IFDEBUG(qDebug() << "ConditionManagerCalendar: Scheduling a timer to" << _nextNearestDateTime.toString() << "interval" << interval);

    timerStart(interval); // , interval + _timerIntervalMaxAddition);
}

void
ConditionManagerCalendar::_updateNextNearestDateTime(const CalendarEntry &entry, const CalendarEntryMatcherDateTime &matcher) {
    QDateTime fromEntry = matcher.nextNearestStartOrEnd(entry);
    if (fromEntry.isValid() && fromEntry < _nextNearestDateTime) {
        _nextNearestDateTime = fromEntry;
        const QDateTime &now = matcher.now();
        _startNextNearestTimer(now);
    }
}

void
ConditionManagerCalendar::_ensureCalendarEntriesLoaded(const QDateTime &now) {
    IFDEBUG(qDebug() << "ConditionManagerCalendar::_ensureCalendarEntriesLoaded _entriesLoadedForDay" << _entriesLoadedForDay.toString());
    if (!_entriesLoadedForDay.isValid() || _entriesLoadedForDay != now.date()) {
        QDate startDate = now.date();
        QDate endDate = startDate.addDays(1);

        // Initializing next nearest invalidation timer so that calendar gets reloaded when day changes
        _nextNearestDateTime = QDateTime(endDate);
        _startNextNearestTimer(now);

        _entries = _calendarManager->loadCalendarEntries(startDate, endDate);
        IFDEBUG(qDebug() << "ConditionManagerCalendar::_ensureCalendarEntriesLoaded loaded" << _entries.size() << "calendar entries");

        _entriesLoadedForDay = now.date();
    }
}
