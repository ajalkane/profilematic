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
#ifndef CONDITIONMANAGERCHARGING_H
#define CONDITIONMANAGERCHARGING_H

#include "conditionmanagercacheable.h"
#include "../platform/platformutil.h"

class ConditionManagerCharging : public ConditionManagerCacheable
{
    Q_OBJECT

    // bool _hasChargingConditions;
    RuleCondition::ChargingState _currentChargingState;

    void _clearVars();
    inline bool _conditionSetForMatching(const RuleCondition &cond) const { return cond.getChargingState() != RuleCondition::UndefinedChargingState; }

public:
    ConditionManagerCharging(QObject *parent = 0);


    virtual bool conditionSetForMatching(const RuleCondition &cond) const;
    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond);

    virtual void startMonitor();
    virtual void stopMonitor();

    virtual void rulesChanged();

//    virtual void startRefresh();
//    virtual bool refresh(const Rule::IdType &, const RuleCondition &rule);
//    virtual void endRefresh();

private slots:
    void batteryChargingStateChanged(int chargingState);

};

#endif // CONDITIONMANAGERCHARGING_H
