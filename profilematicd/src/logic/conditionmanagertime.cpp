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
#include "conditionmanagertime.h"

ConditionManagerTime::ConditionManagerTime(QObject *parent)
    : ConditionManager(parent), _timerIntervalMaxAddition(58)
{
    _timer.setSingleShot(true);

    connect(&_timer, SIGNAL(timeout()), this, SIGNAL(refreshNeeded()));
}

void
ConditionManagerTime::startRefresh(const QDateTime &now) {
    _timer.stop();
    _nextNearestDateTime = QDateTime();
    _refreshTime = now;
}

void
ConditionManagerTime::startRefresh() {
    startRefresh(QDateTime::currentDateTime());
}

void
ConditionManagerTime::endRefresh() {
    if (!_nextNearestDateTime.isNull()) {
        int interval = _refreshTime.secsTo(_nextNearestDateTime);
        qDebug("Now %s", qPrintable(_refreshTime.toString()));
        qDebug("Scheduling a timer to %s, interval %ds", qPrintable(_nextNearestDateTime.toString()), (int)interval);
        _timer.start(interval, interval + _timerIntervalMaxAddition);
    } else {
        qDebug("No nearest time based rule found");
    }
}

bool
ConditionManagerTime::refresh(const Rule &rule) {
    return _refresh(rule, _refreshTime);
}

bool
ConditionManagerTime::_refresh(const Rule &rule, const QDateTime &now) {
    QPair<QDateTime, bool> p = _nextDateTimeFromRule(now, rule);
    QDateTime nearestFromRule = p.first;
    bool matching = p.second;

    qDebug("ConditionManagerTime::refresh rule %s/%s match %d",
           qPrintable(rule.getRuleId()),
           qPrintable(rule.getRuleName()),
           matching);

    if (!nearestFromRule.isNull() && (_nextNearestDateTime.isNull() || nearestFromRule < _nextNearestDateTime)) {
        qDebug("Setting nearest to %s, was %s",
               qPrintable(nearestFromRule.toString()),
               qPrintable(_nextNearestDateTime.toString()));
        _nextNearestDateTime = nearestFromRule;
    }
    return matching;
}

QPair<QDateTime, bool>
ConditionManagerTime::_nextDateTimeFromRule(const QDateTime &from, const Rule &rule) const {
    bool isDaysUsable = rule.isDaysRuleUsable();
    bool isTimeStartUsable = rule.isTimeStartRuleUsable();
    bool isTimeEndUsable = rule.isTimeEndRuleUsable();

    if (!isDaysUsable || !isTimeStartUsable || !isTimeEndUsable) {
        // Return as time matching if time start and time end not set
        bool matching = (!isTimeStartUsable && !isTimeEndUsable);
        qDebug("ConditionManagerTime::time(rule %s) Day or time is not usable, returning null date time (matching %d)",
               qPrintable(rule.getRuleName()), matching);
        return qMakePair(QDateTime(), matching);
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

        qDebug("ConditionManagerTime::time(rule %s), considering dayId %d (%d)", qPrintable(rule.getRuleName()), dayId, considerDay);

        if (considerDay) {
            QDateTime nextStart = from;
            nextStart = nextStart.addDays(i);
            nextStart.setTime(rule.getTimeStart());
            QDateTime nextEnd = _calculateNextEnd(nextStart, rule.getTimeStart(), rule.getTimeEnd());
            qDebug("ConditionManagerTime::from(%s), nextStart(%s), nextEnd(%s)",
                   qPrintable(from.toString()),
                   qPrintable(nextStart.toString()),
                   qPrintable(nextEnd.toString()));

            // Guards against a rule when:
            // - Some days, including current day, are set
            // - No time has been set
            // In other words, a rule that is based on weekdays. In these cases
            // the current day can not be considered as an edge case, but the next day.
            if (nextStart > from) {
                qDebug("ConditionManagerTime::time(rule %s), matching next timeStart returning %s",
                       qPrintable(rule.getRuleName()),
                       qPrintable(nextStart.toString()));
                return qMakePair(nextStart, false);
            } else if (nextEnd >= from) {
                qDebug("ConditionManagerTime::time(rule %s), matching next timeEnd returning %s",
                       qPrintable(rule.getRuleName()),
                       qPrintable(nextEnd.toString()));
                return qMakePair(nextEnd, true);
            }

        }
    }
    qDebug("ConditionManagerTime::time(rule %s), returning null QDateTime",
           qPrintable(rule.getRuleName()));
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
