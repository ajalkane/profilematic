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
#include <QDebug>

#include "conditionmanagercaching.h"

ConditionManagerCaching::ConditionManagerCaching(ConditionManagerCacheable *cacheable, QObject *parent)
    : ConditionManager(parent), _cacheable(cacheable), _numPotentialConditionsInRefresh(0)
{
    _cacheable->setParent(this);
    connect(_cacheable, SIGNAL(matchInvalidated()), this, SLOT(matchInvalidated()));
}

void
ConditionManagerCaching::startRefresh() {
    _numPotentialConditionsInPreviousRefresh = _numPotentialConditionsInRefresh;
    _numPotentialConditionsInRefresh = 0;
}

bool
ConditionManagerCaching::refresh(const Rule::IdType &ruleId, const RuleCondition &rule) {
    ConditionManagerCacheable::MatchStatus match;
    if (_matchCache.contains(ruleId)) {
        match = _matchCache.value(ruleId);
        qDebug() << "ConditionManagerCaching::refresh" << _cacheable->objectName()
                 << "returning cached value " << match;
    } else {
        match = _cacheable->match(ruleId, rule);
        _matchCache.insert(ruleId, match);
    }
    if (match != ConditionManagerCacheable::MatchNotSet) {
        ++_numPotentialConditionsInRefresh;
    }
    return match != ConditionManagerCacheable::NotMatched;
}

void
ConditionManagerCaching::matchedRule(const RuleCondition &rule) {
    Q_UNUSED(rule)
}

void
ConditionManagerCaching::endRefresh() {
    if (_numPotentialConditionsInPreviousRefresh == 0 && _numPotentialConditionsInRefresh > 0) {
        _cacheable->startMonitor();
    } else if (_numPotentialConditionsInPreviousRefresh > 0 && _numPotentialConditionsInRefresh == 0) {
        _cacheable->stopMonitor();
        // Must invalidate cache in this case
        // _matchCache.clear();
    }
}

/**
 * This function is only called when rule is updated. It is not called for removing of rule and adding a rule.
 * For adding a rule, cache do not have to be invalidated as it can't exist in cache yet (generated id is always
 * unique). For removing a rule the cached value will remain for some time, but that does not matter much. It will
 * removed next time matchInvalidated is called.
 */
void
ConditionManagerCaching::ruleUpdated(const Rule &oldRule, const Rule &updatedRule) {
    Q_UNUSED(updatedRule)

    qDebug() << "ConditionManagerCaching::ruleUpdated, invalidating cache for" << _cacheable << oldRule.getRuleName();

    /*
     * Invalidate cache for the rule.
     */
    _matchCache.remove(oldRule.getRuleId());
}

void
ConditionManagerCaching::matchInvalidated() {
    qDebug() << "ConditionManagerCaching::matchInvalidated" << _cacheable->objectName();
    // Clear cache, call refresh
    _matchCache.clear();
    emit refreshNeeded();
}
