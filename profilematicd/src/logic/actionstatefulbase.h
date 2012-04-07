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
#ifndef ACTIONBASE_H
#define ACTIONBASE_H

#include <QString>

#include "action.h"
#include "../model/rule.h"

/**
 * Only calls implementation if ruleId is different than in previous call
 */
class ActionStatefulBase : public Action
{
    Rule::IdType _activeRuleId;
    bool _activeRuleSetInRefresh;

protected:
    /**
     * Helper method to determine if restore previous should be used
     *
     * ruleId: current ruleId that is activated
     * actionSet: true if the action is set for this rule (ie. is not something like "Don't change")
     * restorableActionSet: true if action has been stored for restoring
     */
    inline bool useRestoreAction(const Rule::IdType &ruleId, bool actionSet, bool restorableActionSet) const {
        return (!actionSet || Rule::isDefaultRule(ruleId)) && restorableActionSet;
    }

public:
    ActionStatefulBase();

    virtual void startRefresh();

    void activate(const Rule::IdType &ruleId, const RuleAction &rule);

    virtual void endRefresh();

    // Return true if activated
    virtual bool activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule) = 0;
};

#endif // ACTIONBASE_H
