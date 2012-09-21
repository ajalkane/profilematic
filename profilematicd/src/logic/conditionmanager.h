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
#ifndef CONDITIONMANAGER_H
#define CONDITIONMANAGER_H

#include <QObject>
#include <QList>

#include "../model/rule.h"
#include "../model/rulecondition.h"

class ConditionManager : public QObject
{
    Q_OBJECT

public:
    ConditionManager(QObject *parent = 0);
    virtual ~ConditionManager();

    virtual void startRefresh();
    // Returns true if rule matches current conditions
    virtual bool refresh(const Rule::IdType &ruleId, const RuleCondition &rule) = 0;
    // Called after all rules have been processed, with the Rule that matched all conditions.
    // Called with tthe active rule if:
    //  - The matching Rule is different than the active rule in previous startRefresh/endRefrech cycle
    //  - ProfileMatic is not stopped
    virtual void matchedRule(const RuleCondition &rule);
    virtual void endRefresh();

signals:
    // Signal sent when condition needs refreshing. When emitting this,
    void refreshNeeded();
};

#endif // CONDITIONMANAGER_H
