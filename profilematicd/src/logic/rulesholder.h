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
#ifndef RULESHOLDER_H
#define RULESHOLDER_H

#include <QObject>
#include <QList>
#include <QString>

#include "model/rule.h"

#include "conditionmanager.h"
#include "action.h"

#include "../profileclient.h"

/**
 * Contains the rule data and the objects needed for handling it
 *
 * At the moment, the main purpose is just to keep a list of pointers
 * to only those condition managers that are used for the rule. This
 * avoids calling ConditionManagers that are not needed for a specific
 * rule.
 */
class RuleHolder {
    typedef QList<ConditionManager *> ConditionManagerList;

    QList<ConditionManager *> _conditions;
    // QList<Action *> _actions;
    Rule _rule;

    friend class RulesHolder;

    void _createConditions(const QList<ConditionManager *> cms);

protected:
    RuleHolder(const Rule &rule, const QList<ConditionManager *> cms);

    inline const Rule::IdType ruleId() const { return _rule.getRuleId(); }
    inline const ConditionManagerList conditions() const { return _conditions; }

public:
    RuleHolder &operator=(const RuleHolder &o);

    const Rule &rule() const { return _rule; }
};

class RulesHolder : public QObject
{
    Q_OBJECT

    QList<ConditionManager *> _allConditionManagers;
    QList<Action *> _allActions;

    QList<RuleHolder> _ruleHolders;

    int _findRuleIndexById(const Rule::IdType &id) const;
public:
    RulesHolder(ProfileClient *profileClient);
    virtual ~RulesHolder();

    void startRefresh();
    void endRefresh();

    // Creates a new rule. Returns the new ruleId. If the given rule
    // already has a ruleId, it is not used.
    void appendRule(const Rule &rule);
    void insertRule(int index, const Rule &rule);
    void updateRule(const Rule &rule);
    void removeRule(const Rule::IdType &ruleId);
    void moveRule(const Rule::IdType &ruleId, int toIndex);

    inline int size() const { return _ruleHolders.size(); }
    inline const Rule &ruleAt(int i) const { return _ruleHolders.at(i).rule(); }
    inline const RuleHolder &last() const { return _ruleHolders.last()  ; }

    bool match(const RuleHolder &ruleHolder) const;
    void activate(const RuleHolder &ruleHolder) const;

    inline const QList<RuleHolder> &ruleHolders() const {
        return _ruleHolders;
    }

signals:
    void refreshNeeded();
};

#endif // RULESHOLDER_H
