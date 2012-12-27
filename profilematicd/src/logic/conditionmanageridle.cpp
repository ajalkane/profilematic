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

#include <limits.h>
#include "conditionmanageridle.h"

#define IDLE_MODE_UNKNOWN -1
#define IDLE_MODE_NOT 0
#define IDLE_MODE 1

ConditionManagerIdle::ConditionManagerIdle(QObject *parent)
    : ConditionManagerCacheable(parent),
      _currentMinIdleSecs(INT_MAX),
      _currentIdleMode(IDLE_MODE_UNKNOWN),
      _hasActive(false)
{
    setObjectName("ConditionManagerIdle");
    _timer.setSingleShot(true);
    connect(&_timer, SIGNAL(timeout()), this, SLOT(_timeout()));
}

ConditionManagerIdle::~ConditionManagerIdle() {
}

bool
ConditionManagerIdle::conditionSetForMatching(const RuleCondition &cond) const {
    return _conditionSetForMatching(cond);
}

void
ConditionManagerIdle::startMonitor() {
    qDebug() << "ConditionManagerIdle::startMonitor";
    connect(PlatformUtil::instance(), SIGNAL(userActivityIdleChanged(bool)), this, SLOT(userActivityIdleChanged(bool)));
}

void
ConditionManagerIdle::stopMonitor() {
    qDebug() << "ConditionManagerIdle::stopMonitor";
    disconnect(PlatformUtil::instance(), SIGNAL(userActivityIdleChanged(bool)), this, SLOT(userActivityIdleChanged(bool)));

    _currentIdleMode = IDLE_MODE_UNKNOWN;
    _hasActive = false;
    _timer.stop();
    _idleStartTime = QDateTime();
}

void
ConditionManagerIdle::rulesChanged() {
    qDebug() << "ConditionManagerIdle::rulesChanged";

    _clearVarsForInvalidation();
}

ConditionManagerCacheable::MatchStatus
ConditionManagerIdle::match(const Rule::IdType &ruleId, const RuleCondition &cond) {
    Q_UNUSED(ruleId)

    if (!_conditionSetForMatching(cond)) {
        qDebug() << "ConditionManagerIdle::match() options not set or invalid, matchNotSet";
        return MatchNotSet;
    }

    int idleForSecs = cond.getIdleForSecs();

    _currentMinIdleSecs = qMin(_currentMinIdleSecs, idleForSecs);

    if (_currentIdleMode == IDLE_MODE_UNKNOWN) {
        _currentIdleMode = PlatformUtil::instance()->isUserActivityIdle();
    }

    if (_currentIdleMode == IDLE_MODE_NOT) {
        qDebug() << "ConditionManagerIdle idle mode not active";
        return NotMatched;
    }

    // This check is needed, otherwise idle is kicked into effect immediately.
    // Another approach would be to move idle condition as first condition, but
    // I'm not comfortable with that - the device goes in/out of idle condition
    // often, and it's good that the idle processing can be limited by other
    // rules. The downside is, that idle condition is not totally accurate combined
    // with other rules: Time 21:00 - 06:00 and Idle 30 mins, will mean the earliest
    // activation is 21:30.
    if (_idleStartTime.isNull()) {
        qDebug() << "ConditionManagerIdle::match() idleStartTime is null, initializing to current time";
        _idleStartTime = QDateTime::currentDateTime();
    }

    QDateTime now = QDateTime::currentDateTime();

    int secsToActivation = _secsToIdleWakeup(now, idleForSecs);
    if (secsToActivation <= 0) {
        qDebug() << "ConditionManagerIdle::match() matched, idleForSecs-secsAfterIdle" << secsToActivation;
        _hasActive = true;
        return Matched;
    } else {
        _scheduleWakeup(now, secsToActivation);
        // _nextMinWakeupSecs = qMin(_nextMinWakeupSecs, secsToActivation);
    }
    return NotMatched;
}

void
ConditionManagerIdle::_scheduleWakeup(const QDateTime &now, int secsToWakeUp) {
    QDateTime targetWakeup = now.addSecs(secsToWakeUp);
    qDebug() << "ConditionManagerIdle::_scheduleWakeup() _nextMinWakeUpTime"
             << _nextMinWakeupTime.toString()
             << "targetWakeup"
             << targetWakeup.toString();

    if (_nextMinWakeupTime.isNull() || (now >= _nextMinWakeupTime || targetWakeup < _nextMinWakeupTime)) {
        _nextMinWakeupTime = targetWakeup;
        if (now.time().msec() > 0) secsToWakeUp++;
        int secsToWakeUpEnd = secsToWakeUp + (secsToWakeUp < 30 ? 1 : 30);
        qDebug() << now.toString() << "ConditionManagerIdle::_scheduleWakeup startTimer to"
                 << secsToWakeUp << "-"
                 << secsToWakeUpEnd;

        _timer.start(secsToWakeUp, secsToWakeUpEnd);
        _timerTainted = false;
    }
}

void
ConditionManagerIdle::userActivityIdleChanged(bool isIdle)
{
    // It is typical that device goes in and out of idle.
    // Normally on the other hand the idle timeout is longish,
    // 10 minutes or more. So try to do as little as possible
    // here. This means not touching for example the timer
    // if possible. Rather allow unnecessary timeout signals
    // every now and then.
    qDebug() << "ConditionManagerIdle::userActivityIdleChanged" << isIdle;
    if (isIdle) {
        _idleStartTime = QDateTime::currentDateTime();
        _currentIdleMode = IDLE_MODE;
        if (!_timer.isActive()) {
            _nextMinWakeupTime = QDateTime();
            _scheduleWakeup(_idleStartTime, _currentMinIdleSecs);
        } else {
            _timerTainted = true;
        }
    } else {
        _currentIdleMode = IDLE_MODE_NOT;
        if (_hasActive) {
            qDebug() << QDateTime::currentDateTime().toString() << "ConditionManagerIdle at least one rule was active due to idle, matchInvalidated";
            _clearVarsForInvalidation();
            emit matchInvalidated();
        }
    }
}

void
ConditionManagerIdle::_timeout()
{
    QDateTime now = QDateTime::currentDateTime();
    if (_currentIdleMode == IDLE_MODE) {
        if (!_timerTainted) {
            qDebug() << now.toString() << "ConditionManagerIdle::_timeout on target time "
                     << _nextMinWakeupTime << "when idle, invalidating";
            // _scheduleWakeup(now, _currentMinIdleSecs);
            _clearVarsForInvalidation();
            emit matchInvalidated();
        } else {
            qDebug() << now.toString() << "ConditionManagerIdle::_timeout timer marked tainted, schedule new wakeup";
            int secsToWakeup = _secsToIdleWakeup(now, _currentMinIdleSecs);
            if (secsToWakeup > 0) {
                _nextMinWakeupTime = QDateTime();
                _scheduleWakeup(now, _secsToIdleWakeup(now, _currentMinIdleSecs));
            } else {
                // This can happen with tainted timer, because the wake up time can vary between 0-30 secs. So by the
                // time timeout is called, invalidation should be called instead of scheduling next timeout
                qDebug() << now.toString() << "ConditionManagerIdle::_timeout tainted timeout with negative secsToWakeup"
                         << secsToWakeup << ", must invalidate";
                _clearVarsForInvalidation();
                emit matchInvalidated();
            }
        }
    } else {
        qDebug() << now.toString() << "ConditionManagerIdle::_timeout ignored timeout as not idle"
                 << ", nextMinWakeupTime" << _nextMinWakeupTime.toString();
    }
}

void
ConditionManagerIdle::_clearVarsForInvalidation() {
    _currentMinIdleSecs = INT_MAX;
    _hasActive = false;
    _nextMinWakeupTime = QDateTime();
}
