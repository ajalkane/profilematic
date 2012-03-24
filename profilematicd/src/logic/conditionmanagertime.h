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
//#include <QTimer>
#include <QPair>
#include <QSystemAlignedTimer>

QTM_USE_NAMESPACE

#include "conditionmanager.h"

class ConditionManagerTime : public ConditionManager
{
    typedef ConditionManager super;
    // QTimer _timer;
    QSystemAlignedTimer _timer;
    int _timerIntervalMaxAddition;
    QDateTime _nextNearestDateTime;
    QDateTime _refreshTime;

    QDateTime _calculateNextEnd(const QDateTime &dateTimeStart, const QTime &timeStart, const QTime &timeEnd) const;
    QPair<QDateTime, bool> _nextDateTimeFromRule(const QDateTime &from, const RuleCondition &rule) const;
    bool _refresh(const RuleCondition &rule, const QDateTime &now);

public:
    ConditionManagerTime(QObject *parent = 0);

    virtual void startRefresh();
    virtual bool refresh(const RuleCondition &rule);
    virtual void endRefresh();

    // These functions only needed for unit tests
    const QSystemAlignedTimer *timer() const {
        return &_timer;
    }
    // Minimum interval in msecs
    inline int minimumIntervalMsec() const {
        return _timer.minimumInterval() * 1000;
    }
    // Maximum interval in msecs
    inline int maximumIntervalMsec() const {
        return _timer.maximumInterval() * 1000;
    }
    inline void setTimerMaxIntervalAddition(int add) {
        _timerIntervalMaxAddition = add;
    }

    void startRefresh(const QDateTime &now);
};

#endif // CONDITIONMANAGERTIME_H
