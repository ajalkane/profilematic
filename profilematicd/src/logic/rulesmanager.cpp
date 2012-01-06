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
#include "rulesmanager.h"

RulesManager::RulesManager(const QList<Rule> *rules,
                           ConditionManager *conditionManager,
                           Action *action,
                           const Preferences *preferences,
                           QObject *parent)
    : QObject(parent),
      _rules(rules),
      _conditionManager(conditionManager),
      _action(action),
      _preferences(preferences)
{
}

void
RulesManager::refresh() {
    _conditionManager->startRefresh();
    if (!_preferences->isActive) {
        _conditionManager->refresh(*_rules);
        const QList<Rule> &matchingRules = _conditionManager->matchingRules();

        qDebug("RulesManager::refresh: %d matching rules", matchingRules.size());
        if (!matchingRules.isEmpty()) {
            Rule firstMatchingRule = matchingRules.first();
            _activateRule(firstMatchingRule);
        }
    } else {
        qDebug("RulesManager::refresh(), ProfileMatic not active");
    }
    _conditionManager->endRefresh();
}

void
RulesManager::_activateRule(Rule &rule) {
    qDebug("RulesManager::_activateRule: activatingRule %s/%s",
           qPrintable(rule.getRuleId()),
           qPrintable(rule.getRuleName()));
    _action->activate(rule);
}
