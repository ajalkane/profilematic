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
#ifndef ACTIONACTIVATEONCEBASE_H
#define ACTIONACTIVATEONCEBASE_H

#include <QSet>
#include <QMap>

#include "action.h"
#include "../model/rule.h"

/**
 * Base class for actions that are activated only once
 * during the conditions life-time.
 */
class ActionActivateOnceBase : public Action
{
    QSet<Rule::IdType> _previousRuleIds;
    QSet<Rule::IdType> _matchingRuleIds;
    QMap<Rule::IdType, RuleAction> _previousExitActionsByRuleId;

public:
    ActionActivateOnceBase();

    virtual void startRefresh();

    void activate(const Rule::IdType &ruleId, const RuleAction &rule);

    virtual void endRefresh();

    // Return true if has exit action. False otherwise.
    virtual bool activateOnce(const Rule::IdType &ruleId, const RuleAction &rule) = 0;
    // Called when rule is deactivated.
    virtual void activateOnceExit(const Rule::IdType &ruleId, const RuleAction &rule) = 0;
};

#endif // ACTIONACTIVATEONCEBASE_H
