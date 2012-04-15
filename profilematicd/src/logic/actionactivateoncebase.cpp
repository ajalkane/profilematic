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
#include "actionactivateoncebase.h"

ActionActivateOnceBase::ActionActivateOnceBase()
{
}

void
ActionActivateOnceBase::startRefresh() {
    _previousRuleIds = _activeRuleIds;
    _activeRuleIds.clear();
}

void
ActionActivateOnceBase::activate(const Rule::IdType &ruleId, const RuleAction &rule) {
    _activeRuleIds << ruleId;
    if (!_previousRuleIds.contains(ruleId)) {
        if (activateOnce(ruleId, rule)) {
            _previousExitActionsByRuleId[ruleId] = rule;
        }
    }
}

void
ActionActivateOnceBase::endRefresh() {
    if (!_previousExitActionsByRuleId.isEmpty()) {
        for (QSet<Rule::IdType>::const_iterator i = _previousRuleIds.constBegin();
             i != _previousRuleIds.constEnd(); ++i) {
            if (!_activeRuleIds.contains(*i) &&
                 _previousExitActionsByRuleId.contains(*i)) {
                activateOnceExit(*i, _previousExitActionsByRuleId.value(*i));
                _previousExitActionsByRuleId.remove(*i);
            }
        }
    }
}
