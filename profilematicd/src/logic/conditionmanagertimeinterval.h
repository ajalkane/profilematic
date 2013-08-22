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
#ifndef CONDITIONMANAGERTIMEINTERVAL_H
#define CONDITIONMANAGERTIMEINTERVAL_H

#include <QHash>
#include <QDateTime>

#include "../util/pmtimer.h"

#include "conditionmanagercacheable.h"

class ConditionManagerTimeInterval : public ConditionManagerCacheable
{
    Q_OBJECT

    QDateTime _baseTime;
    // Having baseTimes by ruleId is unnecessary complication as long as
    // ProfileMatic has no support for cleaning it when rule deactivates
    // QHash<Rule::IdType, QDateTime> _baseTimeByRuleId;
    PmTimer _timer;
    QDateTime _nextMinWakeupTime;

    inline bool _conditionSetForMatching(const RuleCondition &cond) const { return cond.timeInterval().isValid(); }

    const QDateTime &_baseTimeForRule(const QDateTime &now, const Rule::IdType &ruleId);
    void _clearVarsForInvalidation();
    void _scheduleWakeUpIfNearest(const QDateTime &now, int wakeUpInSecs);
    void _startNextNearestTimer(const QDateTime &now);

    inline int _relativeTimeWithinPeriod(const QDateTime &now, const RuleConditionTimeInterval &ti, const QDateTime &baseTime) const {
        int periodWindow = ti.getActiveForSecs() + ti.getInactiveForSecs();
        return baseTime.secsTo(now) % periodWindow;
    }

    inline int _relativeTimeForNextActivation(const RuleConditionTimeInterval &ti, int relativeTime) const {
        return (ti.getActiveForSecs() + ti.getInactiveForSecs()) - relativeTime;
    }

    inline int _relativeTimeForNextInactivation(const RuleConditionTimeInterval &ti, int relativeTime) const {
        return ti.getActiveForSecs() - relativeTime;
    }

public:
    ConditionManagerTimeInterval(QObject *parent = 0);

    virtual bool conditionSetForMatching(const RuleCondition &cond) const;
    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond);

    virtual void startMonitor();
    virtual void stopMonitor();

    virtual void rulesChanged();

private slots:
    void _timeout();
};

#endif // CONDITIONMANAGERTIMEINTERVAL_H
