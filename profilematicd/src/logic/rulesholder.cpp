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
    if (!rule.isDefaultRule()) {
        _createConditions(cms);
    }
}

void
RuleHolder::_createConditions(const QList<ConditionManager *> cms) {
    foreach (ConditionManager *m, cms) {
        if (m->conditionSetForMatching(_rule.condition())) {
            qDebug() << "RuleHolder::_createCondition" << m->objectName();
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

    foreach (ConditionManager *m, _allConditionManagers) {
        connect(m, SIGNAL(refreshNeeded()), this, SIGNAL(refreshNeeded()));
    }
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
    qDebug() << "RulesHolder::startRefresh {";
    foreach (ConditionManager *cm, _allConditionManagers) {
        cm->startRefresh();
    }
    foreach (Action *action, _allActions) {
        action->startRefresh();
    }
    qDebug() << "} RulesHolder::startRefresh";
}

void
RulesHolder::endRefresh() {
    qDebug() << "RulesHolder::endRefresh {";
    foreach (ConditionManager *cm, _allConditionManagers) {
        cm->endRefresh();
    }
    foreach (Action *action, _allActions) {
        action->endRefresh();
    }
    qDebug() << "} RulesHolder::endRefresh";
}

void
RulesHolder::appendRule(const Rule &rule) {
    qDebug() << "RulesHolder::appendRule " << rule.getRuleName() << "{";
    _ruleHolders << RuleHolder(rule, _allConditionManagers);
    qDebug() << "} RulesHolder::appendRule";
}

void
RulesHolder::insertRule(int index, const Rule &rule) {
    qDebug() << "RulesHolder::insertRule " << rule.getRuleName() << "{";
    _ruleHolders.insert(index, RuleHolder(rule, _allConditionManagers));
    qDebug() << "} RulesHolder::insertRule";
}

void
RulesHolder::updateRule(const Rule &rule) {
    qDebug() << "RulesHolder::updateRule " << rule.getRuleName() << "{";
    int index = _findRuleIndexById(rule.getRuleId());
    if (index < 0) {
        qWarning() << "RulesHolder::updateRule: Could not find rule with id" << rule.getRuleId();
        qDebug() << "} RulesHolder::updateRule";
        return;
    }

    RuleHolder &updateRuleHolder =_ruleHolders[index];
    foreach (ConditionManager *cm, updateRuleHolder.conditions()) {
        cm->ruleUpdated(updateRuleHolder.rule(), rule);
    }

    _ruleHolders[index] = RuleHolder(rule, _allConditionManagers);
    qDebug() << "} RulesHolder::updateRule";
}

void
RulesHolder::removeRule(const Rule::IdType &ruleId) {
    qDebug() << "RulesHolder::removeRule id" << ruleId << "{";
    int index = _findRuleIndexById(ruleId);
    if (index < 0) {
        qWarning() << "RulesHolder::removeRule: Could not find rule with id" << ruleId;
        qDebug() << "} RulesHolder::removeRule";
        return;
    }

    _ruleHolders.removeAt(index);
    qDebug() << "} RulesHolder::removeRule";
}

void
RulesHolder::moveRule(const Rule::IdType &ruleId, int toIndex) {
    qDebug() << "RulesHolder::moveRule id" << ruleId << "{";
    int fromIndex = _findRuleIndexById(ruleId);
    if (fromIndex < 0) {
        qWarning() << "RulesHolder::moveRule: Could not find rule with id" << ruleId;
        qDebug() << "} RulesHolder::moveRule";
        return;
    }
    _ruleHolders.move(fromIndex, toIndex);
    qDebug() << "} RulesHolder::moveRule";
}

bool
RulesHolder::match(const RuleHolder &ruleHolder) const {
    qDebug() << "RulesHolder::match " << ruleHolder.rule().getRuleName() << "{";
    foreach (ConditionManager *cm, ruleHolder.conditions()) {
        const Rule &rule = ruleHolder.rule();
        if (!cm->refresh(rule.getRuleId(), rule.condition())) {
            qDebug() << "} RulesHolder::match terminated " << rule.getRuleName();
            return false;
        }
    }
    qDebug() << "} RulesHolder::match all matched";
    return true;
}

void
RulesHolder::activate(const RuleHolder &ruleHolder) const {
    qDebug() << "RulesHolder::activatte " << ruleHolder.rule().getRuleName() << "{";
    foreach (Action *action, _allActions) {
        const Rule &rule = ruleHolder.rule();
        action->activate(rule.getRuleId(), rule.action());
    }
    qDebug() << "} RulesHolder::activate";
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
