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
#include "../platform/platformutil.h"

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
    connect(PlatformUtil::instance(), SIGNAL(shuttingDown()), SLOT(shuttingDown()));
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
    qDebug("\n\nREFRESH\n\n");
    _conditionManager->startRefresh();
    _action->startRefresh();
    _matchingRuleIds.clear();
    if (_preferences->isActive) {
        qDebug("%s RulesManager::refresh()", qPrintable(QDateTime::currentDateTime().toString()));
        QList<Rule>::const_iterator ruleI = _rules->constBegin();
        for (; ruleI != _rules->constEnd(); ++ruleI) {
            const Rule &rule = *ruleI;
            if (!rule.getRuleActive()) {
                qDebug("RulesManager::refresh() rule %s is not active, skipping", qPrintable(rule.getRuleName()));
                continue;
            }
            bool isMatching = rule.isDefaultRule() || _conditionManager->refresh(rule.condition());
            if (isMatching) {
                _activateRule(rule);
                _matchingRuleIds << rule.getRuleId();
                _conditionManager->matchedRule(rule.condition());
            }
        }
    } else {
        qDebug("%s RulesManager::refresh(), ProfileMatic not active", qPrintable(QDateTime::currentDateTime().toString()));
    }
    _conditionManager->endRefresh();
    _action->endRefresh();

    emit matchingRuleIdsChanged(_matchingRuleIds.toList());
}

void
RulesManager::_activateRule(const Rule &rule) {
    qDebug("RulesManager::_activateRule: activatingRule %s/%s",
           qPrintable(rule.getRuleId()),
           qPrintable(rule.getRuleName()));
    _action->activate(rule.getRuleId(), rule.action());
}

// Tries upon shutdown to restore the default rule (and restore previous) so that state is not messed up when rebooting the phone.
// This logic assumes activating default rule handles correctly any possible "restore previous" rules. Needs modifications
// if there is changes to that.
void
RulesManager::shuttingDown() {
    qDebug("RulesManager::shuttingDown");
    _conditionManager->startRefresh();
    _action->startRefresh();
    _matchingRuleIds.clear();
    if (_preferences->isActive) {
        qDebug("%s RulesManager::restoring defaults", qPrintable(QDateTime::currentDateTime().toString()));
        // Assume default rule is the last one
        const Rule &defaultRule = _rules->last();
        _activateRule(defaultRule);
        _conditionManager->matchedRule(defaultRule.condition());
    } else {
        qDebug("%s RulesManager::refresh(), ProfileMatic not active", qPrintable(QDateTime::currentDateTime().toString()));
    }
    _conditionManager->endRefresh();
    _action->endRefresh();
}
