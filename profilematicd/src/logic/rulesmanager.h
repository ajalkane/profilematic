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
#ifndef RULESMANAGER_H
#define RULESMANAGER_H

#include <QObject>
#include <QList>
#include <QSet>

#include "conditionmanager.h"
#include "action.h"
#include "rulesholder.h"
#include "../preferences.h"

class RulesManager : public QObject
{
    Q_OBJECT

    RulesHolder *_rulesHolder;
    QSet<Rule::IdType> _matchingRuleIds;

    const Preferences *_preferences;

    void _activateRule(const RuleHolder &ruleHolder);
    void _refresh(bool forceActivate);
    void _matchRule(const RuleHolder &ruleHolder);

public:
    RulesManager(RulesHolder *rules,
                 const Preferences *preferences,
                 QObject *parent = 0);

    // Called when rules have changed
    void refreshRules();
    inline const QSet<Rule::IdType> &matchingRuleIds() const { return _matchingRuleIds; }

signals:
    void matchingRuleIdsChanged(const QStringList &ruleIds);
public slots:
    void refresh();
    void shuttingDown();
};

#endif // RULESMANAGER_H
