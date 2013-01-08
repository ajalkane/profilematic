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

    /**
     * Returns true if the given condition has any rules that could match
     * in some potential situation. Practically this means that user
     * has specified the condition, ie. match returns something else
     * than "MatchNotSet"
     */
    virtual bool conditionSetForMatching(const RuleCondition &cond) const = 0;

    virtual void startRefresh();
    // Returns true if rule matches current conditions
    virtual bool refresh(const Rule::IdType &ruleId, const RuleCondition &rule) = 0;
    virtual void endRefresh();

    // Called (before refresh) when a rule has been updated
    virtual void ruleUpdated(const Rule &oldRule, const Rule &updatedRule);
signals:
    // Signal sent when this condition needs refreshing
    void refreshNeeded();
};

#endif // CONDITIONMANAGER_H
