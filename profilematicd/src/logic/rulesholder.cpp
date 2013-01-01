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

#include "rulesholder.h"

#include "conditionmanagerfactory.h"
#include "actionfactory.h"

RuleHolder::RuleHolder(const Rule &rule, const QList<ConditionManager *> cms)
    : _rule(rule)
{
    _createConditions(cms);
}

void
RuleHolder::_createConditions(const QList<ConditionManager *> cms) {
    foreach (ConditionManager *m, cms) {
        if (m->conditionSetForMatching(_rule.condition())) {
            _conditions << m;
        }
    }
}

RuleHolder &
RuleHolder::operator=(const RuleHolder &o) {
    _conditions = o._conditions;
    _rule = o._rule;

    return *this;
}

RulesHolder::RulesHolder(ProfileClient *profileClient)
{
    _allConditionManagers = ConditionManagerFactory::createAsList();
    _allActions = ActionFactory::createAsList(profileClient);
}

RulesHolder::~RulesHolder() {
    foreach (ConditionManager *m, _allConditionManagers) {
        delete m;
    }
    foreach (Action *a, _allActions) {
        delete a;
    }
}

void
RulesHolder::startRefresh() {
    foreach (ConditionManager *cm, _allConditionManagers) {
        cm->startRefresh();
    }
    foreach (Action *action, _allActions) {
        action->startRefresh();
    }
}

void
RulesHolder::endRefresh() {
    foreach (ConditionManager *cm, _allConditionManagers) {
        cm->endRefresh();
    }
    foreach (Action *action, _allActions) {
        action->endRefresh();
    }
}

void
RulesHolder::appendRule(const Rule &rule) {
    _ruleHolders << RuleHolder(rule, _allConditionManagers);
}

void
RulesHolder::updateRule(const Rule &rule) {
    int index = _findRuleIndexById(rule.getRuleId());
    if (index < 0) {
        qWarning() << "RulesHolder::updateRule: Could not find rule with id" << rule.getRuleId();
        return;
    }

    _ruleHolders[index] = RuleHolder(rule, _allConditionManagers);
}

void
RulesHolder::removeRule(const Rule::IdType &ruleId) {
    int index = _findRuleIndexById(ruleId);
    if (index < 0) {
        qWarning() << "RulesHolder::updateRule: Could not find rule with id" << ruleId;
        return;
    }

    _ruleHolders.removeAt(index);
}

void
RulesHolder::moveRule(int fromIndex, int toIndex) {
    _ruleHolders.move(fromIndex, toIndex);
}

bool
RulesHolder::match(const RuleHolder &ruleHolder) const {
    foreach (ConditionManager *cm, ruleHolder.conditions()) {
        const Rule &rule = ruleHolder.rule();
        if (!cm->refresh(rule.getRuleId(), rule.condition())) {
            return false;
        }
    }
    return true;
}

void
RulesHolder::activate(const RuleHolder &ruleHolder) const {
    foreach (Action *action, _allActions) {
        const Rule &rule = ruleHolder.rule();
        action->activate(rule.getRuleId(), rule.action());
    }
}

int
RulesHolder::_findRuleIndexById(const Rule::IdType &id) const {
    for (int i = 0; i < _ruleHolders.size(); ++i) {
        const RuleHolder &r = _ruleHolders.at(i);
        if (id == r.ruleId()) {
            return i;
        }
    }
    return -1;
}
