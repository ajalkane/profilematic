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
#include "conditionmanagerchain.h"

ConditionManagerChain::ConditionManagerChain(QObject *parent)
    : ConditionManager(parent)
{
}

ConditionManagerChain::~ConditionManagerChain()
{
    while (!_conditionManagers.isEmpty()) {
        ConditionManager *cm = _conditionManagers.back();
        _conditionManagers.pop_back();
        delete cm;
    }
}

bool
ConditionManagerChain::conditionSetForMatching(const RuleCondition &cond) const {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        if (cm->conditionSetForMatching(cond)) return false;
    }
    return true;
}

void
ConditionManagerChain::startRefresh() {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        cm->startRefresh();
    }
}

bool
ConditionManagerChain::refresh(const Rule::IdType &ruleId, const RuleCondition &rule) {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        if (!cm->refresh(ruleId, rule)) {
            return false;
        }
    }
    return true;
}

void
ConditionManagerChain::matchedRule(const RuleCondition &rule) {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        cm->matchedRule(rule);
    }
}

void
ConditionManagerChain::endRefresh() {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        cm->endRefresh();
    }
}

void
ConditionManagerChain::ruleUpdated(const Rule &oldRule, const Rule &updatedRule) {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        cm->ruleUpdated(oldRule, updatedRule);
    }
}

void
ConditionManagerChain::add(ConditionManager *conditionManager) {
    _conditionManagers.append(conditionManager);
    connect(conditionManager, SIGNAL(refreshNeeded()), this, SIGNAL(refreshNeeded()));
}
