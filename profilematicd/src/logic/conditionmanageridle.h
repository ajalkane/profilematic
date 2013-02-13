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
#ifndef CONDITIONMANAGERIDLE_H
#define CONDITIONMANAGERIDLE_H

#include <QDateTime>
#include <QTime>

#include "../util/pmtimer.h"

#include "conditionmanagercacheable.h"
#include "../platform/platformutil.h"

class ConditionManagerIdle : public ConditionManagerCacheable
{
    Q_OBJECT

    PmTimer _timer;
    int _currentMinIdleSecs;
    int _currentIdleSecs;
    // int _nextMinWakeupSecs;
    int _currentIdleMode;
    bool _hasActive;
    QDateTime _nextMinWakeupTime;
    QDateTime _idleStartTime;
    bool _timerTainted;

    // Returns true if new wakeup was scheduled. False if earlier was already scheduled.
    void _scheduleWakeup(const QDateTime &now, int secsToWakeUp);
    inline int  _secsToIdleWakeup(const QDateTime &now, int idleForSecs) { return idleForSecs - _idleStartTime.secsTo(now); }
    inline bool _conditionSetForMatching(const RuleCondition &cond) const { return cond.getIdleForSecs() >= 0; }

    void _clearVarsForInvalidation();
public:
    ConditionManagerIdle(QObject *parent = 0);
    virtual ~ConditionManagerIdle();

    virtual bool conditionSetForMatching(const RuleCondition &cond) const;
    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond);

    virtual void startMonitor();
    virtual void stopMonitor();

    virtual void rulesChanged();

//    virtual void startRefresh();
//    virtual bool refresh(const Rule::IdType &, const RuleCondition &rule);
//    virtual void endRefresh();

private slots:
    void userActivityIdleChanged(bool isIdle);
    void _timeout();
};

#endif // CONDITIONMANAGERIDLE_H
