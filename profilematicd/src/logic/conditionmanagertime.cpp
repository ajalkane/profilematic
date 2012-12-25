/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
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
#include "conditionmanagertime.h"

ConditionManagerTime::ConditionManagerTime(QObject *parent)
    : ConditionManagerCacheable(parent), _timerIntervalMaxAddition(30)
{
    IFDEBUG(qDebug() << "ConditionManagerTime::constructor");

    setObjectName("ConditionManagerTime");

    _timer.setSingleShot(true);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(_timeout()));
}

bool
ConditionManagerTime::conditionSetForMatching(const RuleCondition &cond) const {
    return _conditionSetForMatching(cond);
}

ConditionManagerCacheable::MatchStatus
ConditionManagerTime::match(const Rule::IdType &, const RuleCondition &cond) {
    QDateTime now = QDateTime::currentDateTime();
    return match(now, cond);
}

ConditionManagerCacheable::MatchStatus
ConditionManagerTime::match(const QDateTime &now, const RuleCondition &cond) {
    if (!_conditionSetForMatching(cond)) {
        IFDEBUG(qDebug() << "ConditionManagerTime::match() time options not set or invalid, matches");
        return MatchNotSet;
    }

    QPair<QDateTime, bool> p = _nextDateTimeFromRule(now, cond);
    const QDateTime &nearestFromRule = p.first;
    bool matching = p.second;

    IFDEBUG(qDebug() << "ConditionManagerTime::match " << matching);

    _updateNextNearestDateTime(now, nearestFromRule);
    return matching ? Matched : NotMatched;
}

void
ConditionManagerTime::startMonitor() {
    // NO-OP
}

void
ConditionManagerTime::stopMonitor() {
    IFDEBUG(qDebug() << "ConditionManagerTime::stopMonitor stoppingTimer");
    _timer.stop();
    _nextNearestDateTime = QDateTime();
}

void
ConditionManagerTime::rulesChanged() {
    IFDEBUG(qDebug() << "ConditionManagerTime::rulesChanged stoppingTimer");
    _timer.stop();
    _nextNearestDateTime = QDateTime();
}

void
ConditionManagerTime::_updateNextNearestDateTime(const QDateTime &now, const QDateTime &nearestFromRule ) {
    if (!nearestFromRule.isNull()) {
        if (now >= _nextNearestDateTime) {
            IFDEBUG(qDebug() << "ConditionManagerTime::now >= _nextNearestDateTime, resetting for new calculation");
            _nextNearestDateTime = QDateTime();
        }
        if (_nextNearestDateTime.isNull() || nearestFromRule < _nextNearestDateTime) {
            IFDEBUG(qDebug() << "ConditionManagerTime::Setting nearest to" << nearestFromRule.toString() << "was" << _nextNearestDateTime.toString());
            _nextNearestDateTime = nearestFromRule;
            _startNextNearestTimer(now);
        }
    }
}

void
ConditionManagerTime::_startNextNearestTimer(const QDateTime &now) {
    int interval = now.secsTo(_nextNearestDateTime);
    if (now.time().msec() > 0) interval += 1;
    IFDEBUG(qDebug() << "ConditionManagerTime: Now" << now.toString();
            qDebug() << "ConditionManagerTime: Scheduling a timer to" << _nextNearestDateTime.toString() << ", interval" << interval);

    _timer.start(interval, interval + _timerIntervalMaxAddition);
}

QPair<QDateTime, bool>
ConditionManagerTime::_nextDateTimeFromRule(const QDateTime &from, const RuleCondition &rule) const {
    if (!rule.isTimeConditionValid()) {
        IFDEBUG(qDebug() << "ConditionManagerTime::time Day or time is not usable, returning null date time");
        return qMakePair(QDateTime(), false);
    }

    // rule.daysActive = true and days not empty always if gets this far
    const QSet<int> &selectedDays = rule.getDays();
    int dayOfWeek = from.date().dayOfWeek();
    // i goes up to 7, so that next week's current day is considered also.
    // i starts from -1, to start from previous day, so that matches endTime for example
    // in a case when startTime = 22:00, endTime = 06:00 and now = 02:00
    for (int i = -1; i < 8; ++i) {
        // Adding 7 to i so that dayId can never be negative
        int dayId = (dayOfWeek - 1 + i + 7) % 7;
        bool considerDay = selectedDays.contains(dayId);

        IFDEBUG(qDebug() << "ConditionManagerTime::time, considering dayId" << dayId << "(" << considerDay << ")");

        if (considerDay) {
            QDateTime nextStart = from;
            nextStart = nextStart.addDays(i);
            nextStart.setTime(rule.getTimeStart());
            QDateTime nextEnd = _calculateNextEnd(nextStart, rule.getTimeStart(), rule.getTimeEnd());
            IFDEBUG(qDebug() << "ConditionManagerTime::from" << from.toString()
                     << ", nextStart" << nextStart.toString()
                     << ", nextEnd" << nextEnd.toString());

            // Guards against a rule when:
            // - Some days, including current day, are set
            // - No time has been set
            // In other words, a rule that is based on weekdays. In these cases
            // the current day can not be considered as an edge case, but the next day.
            if (nextStart > from) {
                IFDEBUG(qDebug() << "ConditionManagerTime::time, matching next timeStart returning" << nextStart.toString();
                return qMakePair(nextStart, false));
            } else if (nextEnd >= from) {
                IFDEBUG(qDebug() << "ConditionManagerTime::time, matching next timeEnd returning" << nextEnd.toString());
                return qMakePair(nextEnd, true);
            }

        }
    }
    IFDEBUG(qDebug() << "ConditionManagerTime::time returning null QDateTime");
    // Means no time based rules
    return qMakePair(QDateTime(), true);
}

QDateTime
ConditionManagerTime::_calculateNextEnd(const QDateTime &dateTimeStart, const QTime &timeStart, const QTime &timeEnd) const {
    QDateTime nextEnd(dateTimeStart);
    nextEnd.setTime(timeEnd);
    if (timeEnd <= timeStart) {
        nextEnd = nextEnd.addDays(1);
    }
    return nextEnd;
}

void
ConditionManagerTime::_timeout() {
    QDateTime now = QDateTime::currentDateTime();
    IFDEBUG(qDebug() << now << "ConditionManagerTime::_timeout";
            qDebug() << "ConditionManagerTime::_timeout msec" << now.time().msec());
    _nextNearestDateTime = QDateTime();
    emit matchInvalidated();
}
