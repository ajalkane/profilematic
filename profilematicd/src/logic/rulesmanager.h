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
#include "../preferences.h"
#include "../model/rule.h"

class RulesManager : public QObject
{
    Q_OBJECT

    const QList<Rule> *_rules;
    QSet<Rule::IdType> _activeRuleIds;

    ConditionManager *_conditionManager;
    Action           *_action;
    const Preferences *_preferences;

    void _activateRule(const Rule &rule);
    void _refresh(bool forceActivate);
public:
    RulesManager(const QList<Rule> *rules,
                 ConditionManager *conditionManager,
                 Action *action,
                 const Preferences *preferences,
                 QObject *parent = 0);

    // Called when rules have changed
    void refreshRules();
    inline const QSet<Rule::IdType> &activeRuleIds() const { return _activeRuleIds; }

signals:
    void activeRuleIdsChanged(const QStringList &ruleIds);
public slots:
    void refresh();
    void shuttingDown();
};

#endif // RULESMANAGER_H
