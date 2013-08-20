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

#include "conditionmanagertimeinterval.h"

ConditionManagerTimeInterval::ConditionManagerTimeInterval(QObject *parent)
    : ConditionManagerCacheable(parent)
{
    setObjectName("ConditionManagerTimeInterval");
    _timer.setSingleShot(true);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(_timeout()));
}

bool
ConditionManagerTimeInterval::conditionSetForMatching(const RuleCondition &cond) const {
    return _conditionSetForMatching(cond);
}

void
ConditionManagerTimeInterval::startMonitor() {
    IFDEBUG(qDebug() << Q_FUNC_INFO);
}

void
ConditionManagerTimeInterval::stopMonitor() {
    IFDEBUG(qDebug() << Q_FUNC_INFO);

    _clearVarsForInvalidation();
    _baseTimeByRuleId.clear();
}

void
ConditionManagerTimeInterval::rulesChanged() {
    IFDEBUG(qDebug() << Q_FUNC_INFO);

    _clearVarsForInvalidation();
}

ConditionManagerCacheable::MatchStatus
ConditionManagerTimeInterval::match(const Rule::IdType &ruleId, const RuleCondition &cond) {
    if (!_conditionSetForMatching(cond)) {
        IFDEBUG(qDebug() << Q_FUNC_INFO << "options not set or invalid, matchNotSet");
        return MatchNotSet;
    }

    QDateTime now = QDateTime::currentDateTime();
    QDateTime baseTime = _baseTimeForRule(now, ruleId);
    const RuleConditionTimeInterval &ti = cond.timeInterval();
    int relativeTime = _relativeTimeWithinPeriod(now, ti, baseTime);

    IFDEBUG(qDebug() << Q_FUNC_INFO << "now:" << now << "baseTime:" << baseTime << "relativeTime:" << relativeTime);

    if (relativeTime < ti.getActiveForSecs()) {
        IFDEBUG(qDebug() << Q_FUNC_INFO << "matching");
        int nextInactivationInSecs = _relativeTimeForNextInactivation(ti, relativeTime);
        _scheduleWakeUpIfNearest(now, nextInactivationInSecs);
        return Matched;
    } else {
        IFDEBUG(qDebug() << Q_FUNC_INFO << "not matching");
        int nextActivationInSecs = _relativeTimeForNextActivation(ti, relativeTime);
        _scheduleWakeUpIfNearest(now, nextActivationInSecs);
    }

    return NotMatched;
}

QDateTime
ConditionManagerTimeInterval::_baseTimeForRule(const QDateTime &now, const Rule::IdType &ruleId) {
    if (_baseTimeByRuleId.contains(ruleId)) {
        return _baseTimeByRuleId.value(ruleId);
    }
    _baseTimeByRuleId.insert(ruleId, now);

    return now;
}

void
ConditionManagerTimeInterval::_scheduleWakeUpIfNearest(const QDateTime &now, int wakeUpInSecs) {
    QDateTime nextWakeUp = now.addSecs(wakeUpInSecs);

    IFDEBUG(qDebug() << Q_FUNC_INFO << "wakeUpInSecs:" << wakeUpInSecs << "nextWakeUp:" << nextWakeUp << "_nextMinWakeUp:" << _nextMinWakeupTime);
    if (_nextMinWakeupTime.isNull() || nextWakeUp < _nextMinWakeupTime) {
        _nextMinWakeupTime = nextWakeUp;
        _startNextNearestTimer(now);
    }

}

void
ConditionManagerTimeInterval::_startNextNearestTimer(const QDateTime &now) {
    int secsToWakeUp = now.secsTo(_nextMinWakeupTime);

    if (now.time().msec() > 0) secsToWakeUp++;
    int secsToWakeUpEnd = secsToWakeUp + (secsToWakeUp < 30 ? 1 : 30);
    IFDEBUG(qDebug() << Q_FUNC_INFO << now << "startTimer to" << secsToWakeUp << "-" << secsToWakeUpEnd);

    _timer.start(secsToWakeUp, secsToWakeUpEnd);
}

void
ConditionManagerTimeInterval::_clearVarsForInvalidation() {
    _timer.stop();
    _nextMinWakeupTime = QDateTime();
}

void
ConditionManagerTimeInterval::_timeout() {
    _clearVarsForInvalidation();
    emit matchInvalidated();
}
