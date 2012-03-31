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
    connect(_conditionManager, SIGNAL(refreshNeeded()), this, SLOT(refresh()));
}

void
RulesManager::refreshRules() {
    _refresh(true);
}

void
RulesManager::refresh() {
    _refresh(false);
}

void
RulesManager::_refresh(bool forceActivate) {
    _conditionManager->startRefresh();
    if (_preferences->isActive) {
        qDebug("%s RulesManager::refresh()", qPrintable(QDateTime::currentDateTime().toString()));
        QList<Rule>::const_iterator firstMatchingRule = _rules->constBegin();
        for (; firstMatchingRule != _rules->constEnd(); ++firstMatchingRule) {
            const Rule &rule = *firstMatchingRule;
            bool isMatching = rule.isDefaultRule() || _conditionManager->refresh(rule.condition());
            if (isMatching) {
                break;
            }
        }

        qDebug("%s RulesManager::refresh matching rule found %d, is same as current %d, is forceActivate %d",
               qPrintable(QDateTime::currentDateTime().toString()),
               firstMatchingRule != _rules->constEnd(),
               _activeRuleIds.contains(firstMatchingRule->getRuleId()),
               forceActivate);

        if (firstMatchingRule != _rules->constEnd() && (forceActivate || !_activeRuleIds.contains(firstMatchingRule->getRuleId()))) {
            const Rule &matchedRule = *firstMatchingRule;
            _conditionManager->matchedRule(matchedRule.condition());
            _activateRule(matchedRule);
            _activeRuleIds.clear();
            _activeRuleIds << matchedRule.getRuleId();
            emit activeRuleIdsChanged(_activeRuleIds.toList());
        }
    } else {
        qDebug("%s RulesManager::refresh(), ProfileMatic not active", qPrintable(QDateTime::currentDateTime().toString()));
        _activeRuleIds.clear();
    }
    _conditionManager->endRefresh();
}

void
RulesManager::_activateRule(const Rule &rule) {
    qDebug("RulesManager::_activateRule: activatingRule %s/%s",
           qPrintable(rule.getRuleId()),
           qPrintable(rule.getRuleName()));
    _action->activate(rule.action());
}
