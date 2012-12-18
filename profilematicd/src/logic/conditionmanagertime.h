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
#ifndef CONDITIONMANAGERTIME_H
#define CONDITIONMANAGERTIME_H

#include <QDateTime>
#include <QTime>
#include <QPair>

#include "../util/pmtimer.h"
#include "conditionmanagercacheable.h"

class ConditionManagerTime : public ConditionManagerCacheable
{
    Q_OBJECT

    typedef ConditionManagerCacheable super;
    PmTimer _timer;
    QDateTime _nextNearestDateTime;
    int _timerIntervalMaxAddition;


    inline bool _conditionSetForMatching(const RuleCondition &cond) const { return cond.isTimeConditionValid(); }

    void _updateNextNearestDateTime(const QDateTime &now, const QDateTime &nearestFromRule);
    void _startNextNearestTimer(const QDateTime &now);

    QPair<QDateTime, bool> _nextDateTimeFromRule(const QDateTime &from, const RuleCondition &rule) const;
    QDateTime _calculateNextEnd(const QDateTime &dateTimeStart, const QTime &timeStart, const QTime &timeEnd) const;

public:
    ConditionManagerTime(QObject *parent = 0);

    virtual bool conditionSetForMatching(const RuleCondition &cond) const;
    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond);

    virtual void startMonitor();
    virtual void stopMonitor();

    virtual void rulesChanged();

    // These functions only needed for unit tests
    MatchStatus match(const QDateTime &now, const RuleCondition &cond);

    inline PmTimer *timer() {
        return &_timer;
    }
    // Minimum interval in msecs
    inline int minimumIntervalSec() const {
        return _timer.minimumInterval();
    }
    // Maximum interval in msecs
    inline int maximumIntervalSec() const {
        return _timer.maximumInterval();
    }
    inline void setTimerMaxIntervalAddition(int add) {
        _timerIntervalMaxAddition = add;
    }

private slots:
    void _timeout();

    friend class TestConditionManagerTime;
};

#endif // CONDITIONMANAGERTIME_H
