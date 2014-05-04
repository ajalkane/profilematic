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

#include "rulesmanager.h"
#include "../platform/platformutil.h"

RulesManager::RulesManager(RulesHolder *rulesHolder,
                           const Preferences *preferences,
                           QObject *parent)
    : QObject(parent),
      _rulesHolder(rulesHolder),
      _preferences(preferences)
{
    connect(_rulesHolder, SIGNAL(refreshNeeded()), this, SLOT(refresh()));
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
RulesManager::_refresh(bool /*forceActivate*/) {
    IFDEBUG(qDebug("\n\nREFRESH\n\n"));
    _rulesHolder->startRefresh();
    const QSet<Rule::IdType> previouslyMatchingRuleIds(_matchingRuleIds);
    _matchingRuleIds.clear();
    if (_preferences->isActive) {
        IFDEBUG(qDebug("%s RulesManager::refresh()", qPrintable(QDateTime::currentDateTime().toString())));
        QList<RuleHolder>::const_iterator ruleI = _rulesHolder->ruleHolders().constBegin();
        QList<RuleHolder>::const_iterator ruleE = _rulesHolder->ruleHolders().constEnd();

        for (; ruleI != ruleE; ++ruleI) {
            const RuleHolder &ruleHolder = *ruleI;
            const Rule &rule = ruleHolder.rule();
            if (rule.isDefaultRule()) continue;

            if (!rule.getRuleActive()) {
                IFDEBUG(qDebug("\nRulesManager::refresh() rule %s is not active, skipping", qPrintable(rule.getRuleName())));
                continue;
            }
            IFDEBUG(qDebug() << "\nRulesManager::refresh() considering rule" << rule.getRuleName());
            bool isMatching = _rulesHolder->match(ruleHolder);
            if (isMatching) {
                _matchRule(ruleHolder, previouslyMatchingRuleIds);
                if (rule.getStopIfMatched()) {
                    IFDEBUG(qDebug("RulesManager::refresh(), rule has stopIfMatched, skipping rest"));
                    break;
                }
            }
        }
        // Assume default rule is the last one
        const RuleHolder &defaultRuleHolder = _rulesHolder->last();
        // Default rule always matches
        _matchRule(defaultRuleHolder, previouslyMatchingRuleIds);
    } else {
        IFDEBUG(qDebug("%s RulesManager::refresh(), ProfileMatic not active", qPrintable(QDateTime::currentDateTime().toString())));
    }
    _rulesHolder->endRefresh();

    // Conditionally if has been set in preferences
    // _notifyOfNewMatchingRules(previouslyMatchingRuleIds);

    if (previouslyMatchingRuleIds != _matchingRuleIds) {
        emit matchingRuleIdsChanged(_matchingRuleIds.toList());
    }

    IFDEBUG(qDebug("\n\nEND REFRESH\n\n"));
}

void
RulesManager::_notifyOfNewMatchingRule(const Rule &rule) {
    // TODO: translations
    PlatformUtil::instance()->publishNotification(rule.getRuleName() + " activated");
}

void
RulesManager::_matchRule(const RuleHolder &ruleHolder, const QSet<Rule::IdType> &previouslyMatchingRuleIds) {
    _activateRule(ruleHolder);
    _matchingRuleIds << ruleHolder.rule().getRuleId();

    const Rule &rule = ruleHolder.rule();
    if (!previouslyMatchingRuleIds.contains(rule.getRuleId())) {
        _notifyOfNewMatchingRule(rule);
    }
}

void
RulesManager::_activateRule(const RuleHolder &ruleHolder) {
    const Rule &rule = ruleHolder.rule();
    IFDEBUG(qDebug("RulesManager::_activateRule: activatingRule %s/%s",
           qPrintable(rule.getRuleId()),
           qPrintable(rule.getRuleName())));
    _rulesHolder->activate(ruleHolder);
}

// Tries upon shutdown to restore the default rule (and restore previous) so that state is not messed up when rebooting the phone.
// This logic assumes activating default rule handles correctly any possible "restore previous" rules. Needs modifications
// if there is changes to that.
void
RulesManager::shuttingDown() {
    IFDEBUG(qDebug("RulesManager::shuttingDown"));
    _rulesHolder->startRefresh();
    _matchingRuleIds.clear();
    if (_preferences->isActive) {
        IFDEBUG(qDebug("%s RulesManager::restoring defaults", qPrintable(QDateTime::currentDateTime().toString())));
        // Assume default rule is the last one
        const RuleHolder &defaultRule = _rulesHolder->last();
        _activateRule(defaultRule);
    } else {
        IFDEBUG(qDebug("%s RulesManager::refresh(), ProfileMatic not active", qPrintable(QDateTime::currentDateTime().toString())));
    }
    _rulesHolder->endRefresh();
}
