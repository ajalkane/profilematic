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
RulesManager::refresh() {
    _conditionManager->startRefresh();
    if (_preferences->isActive) {
        if (_rules->size() != 1) {
            QList<Rule>::const_iterator firstMatchingRule = _rules->constBegin();
            for (; firstMatchingRule != _rules->constEnd(); ++firstMatchingRule) {
                const Rule &rule = *firstMatchingRule;
                bool isMatching = rule.isDefaultRule() || _conditionManager->refresh(*firstMatchingRule);
                if (isMatching) {
                    break;
                }
            }

            if (firstMatchingRule != _rules->constEnd()) {
                _activateRule(*firstMatchingRule);
            }
        } else {
            qDebug("RulesManager::refresh(), only default rule exists, refresh not done");
        }
    } else {
        qDebug("RulesManager::refresh(), ProfileMatic not active");
    }
    _conditionManager->endRefresh();
}

void
RulesManager::_activateRule(const Rule &rule) {
    qDebug("RulesManager::_activateRule: activatingRule %s/%s",
           qPrintable(rule.getRuleId()),
           qPrintable(rule.getRuleName()));
    _action->activate(rule);
}
