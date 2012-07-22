/**********************************************************************
 * Copyright 2011 Arto Jalkanen
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
#ifndef RULE_H
#define RULE_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QTime>
#include <QMetaType>
#include <QDBusArgument>

#include "rulecondition.h"
#include "ruleaction.h"

#define DEFAULT_RULE_ID "defaultRule"

// IMPROVE: conditions to separate class
// IMPROVE: it'd be cleaner to have RuleAction as member of Rule, but inheriting from it
// will be less painful transition
class Rule : public QObject
{
    Q_OBJECT

    QString   _ruleId;
    QString   _ruleName;
    bool      _ruleActive;

    RuleCondition _condition;
    RuleAction _action;

    Q_PROPERTY(QString ruleId READ getRuleId NOTIFY ruleIdChanged)
    Q_PROPERTY(QString ruleName READ getRuleName WRITE setRuleName NOTIFY ruleNameChanged)
    Q_PROPERTY(bool ruleActive READ getRuleActive WRITE setRuleActive NOTIFY ruleActiveChanged)
    Q_PROPERTY(bool isDefaultRule READ isDefaultRule NOTIFY ruleIdChanged STORED false)
    Q_PROPERTY(RuleCondition *condition READ conditionQml NOTIFY conditionChanged STORED false)
    Q_PROPERTY(RuleAction *action READ actionQml NOTIFY actionChanged STORED false)

    void _init();
signals:
    void changed();

    void conditionChanged();
    void actionChanged();

    void ruleNameChanged();
    void ruleIdChanged();
    void ruleActiveChanged();
public:
    typedef QString IdType;

    Rule(QObject *parent = 0);
    Rule(const Rule &o);
    static Rule createDefaultRule();

    virtual ~Rule();
    Rule &operator=(const Rule &o);
    Rule &conditionsFrom(const Rule &o);
    Rule &actionsFrom(const Rule &o);
    bool isDefaultRule() const;
    static inline bool isDefaultRule(const IdType &id) { return id == DEFAULT_RULE_ID; }

    const QString &getRuleId() const;
    void setRuleId(const QString &ruleId);

    QString getRuleName() const;
    void setRuleName(const QString &ruleName);

    inline bool getRuleActive() const { return _ruleActive; }
    void setRuleActive(bool active);

    inline       RuleAction &action()             { return _action; }
    inline       RuleCondition &condition()       { return _condition; }
    inline const RuleAction &action()       const { return _action; }
    inline const RuleCondition &condition() const { return _condition; }

    inline RuleCondition *conditionQml() { return &_condition; }
    inline RuleAction *actionQml() { return &_action; }

    inline bool operator==(const Rule &o) const {
        return this->_ruleId     == o._ruleId
            && this->_ruleName   == o._ruleName
            && this->_ruleActive == o._ruleActive
            && this->_condition  == o._condition
            && this->_action     == o._action;
    }
};

Q_DECLARE_METATYPE(Rule)
Q_DECLARE_METATYPE(QList<Rule>)

// Marshall the Rule data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const Rule &mystruct);

// Retrieve the Rule data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, Rule &mystruct);

#endif // RULE_H
