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
#ifndef CONDITIONMANAGERCACHING_H
#define CONDITIONMANAGERCACHING_H

#include <QHash>

#include "conditionmanager.h"
#include "conditionmanagercacheable.h"

class ConditionManagerCaching : public ConditionManager
{
    Q_OBJECT

    ConditionManagerCacheable *_cacheable;

    QHash<Rule::IdType, ConditionManagerCacheable::MatchStatus> _matchCache;

    int _numPotentialConditionsInRefresh;
    int _numPotentialConditionsInPreviousRefresh;
public:
    /**
     * Takes ownership of the given Cacheable object
     */
    ConditionManagerCaching(ConditionManagerCacheable *cacheable, QObject *parent = 0);

    virtual bool conditionSetForMatching(const RuleCondition &cond) const;

    virtual void startRefresh();
    virtual bool refresh(const Rule::IdType &ruleId, const RuleCondition &rule);
    virtual void matchedRule(const RuleCondition &rule);
    virtual void endRefresh();
    virtual void ruleUpdated(const Rule &oldRule, const Rule &updatedRule);

private slots:
    void matchInvalidated();
};

#endif // CONDITIONMANAGERCACHING_H
