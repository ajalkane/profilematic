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
#include "rule.h"

Rule::Rule(QObject *parent)
    : QObject(parent), _ruleActive(true)
{
    _init();
}

Rule::Rule(const Rule &o)
    : QObject(NULL),
      _ruleId(o._ruleId),
      _ruleName(o._ruleName),
      _ruleActive(o._ruleActive),
      _condition(o.condition()), _action(o.action())
{
    _init();
}

Rule &
Rule::operator=(const Rule &o) {
    _ruleId = o._ruleId;
    _ruleName = o._ruleName;
    _ruleActive = o._ruleActive;
    _condition = o._condition;
    _action = o._action;
    return *this;
}

void
Rule::_init() {
    connect(this,        SIGNAL(ruleNameChanged()),  this, SIGNAL(changed()));
    connect(this,        SIGNAL(ruleIdChanged()),    this, SIGNAL(changed()));
    connect(this,        SIGNAL(ruleActiveChanged()),    this, SIGNAL(changed()));
    connect(&_condition, SIGNAL(changed()),          this, SIGNAL(conditionChanged()));
    connect(&_action,    SIGNAL(changed()),          this, SIGNAL(actionChanged()));
    connect(&_condition, SIGNAL(changed()), this, SIGNAL(changed()));
    connect(&_action,     SIGNAL(changed()),    this, SIGNAL(changed()));
//    connect(this,        SIGNAL(conditionChanged()), this, SIGNAL(changed()));
//    connect(this,        SIGNAL(actionChanged()),    this, SIGNAL(changed()));
}

Rule
Rule::createDefaultRule() {
    Rule r = Rule();
    r.setRuleId(DEFAULT_RULE_ID);
    r.setRuleName("Default rule");
    return r;
}

Rule::~Rule() {}

Rule &
Rule::conditionsFrom(const Rule &o) {
    _condition = o.condition();
    return *this;
}

Rule &
Rule::actionsFrom(const Rule &o) {
    _action = o.action();
    return *this;
}

bool
Rule::isDefaultRule() const {
    return QLatin1String("defaultRule") == _ruleId;
}

const QString &
Rule::getRuleId() const {
    return _ruleId;
}

void
Rule::setRuleId(const QString &ruleId) {
    if (_ruleId != ruleId) {
        _ruleId = ruleId;
        emit ruleIdChanged();
    }
}

QString
Rule::getRuleName() const {
    return _ruleName;
}

void
Rule::setRuleName(const QString &ruleName) {
    if (_ruleName != ruleName) {
        _ruleName = ruleName;
        emit ruleNameChanged();
    }
}

void
Rule::setRuleActive(bool active) {
    if (_ruleActive != active) {
        _ruleActive = active;
        emit ruleActiveChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const Rule &rule)
{
    argument.beginStructure();
    argument << rule.getRuleId();
    argument << rule.getRuleName();
    argument << rule.getRuleActive();
    argument << rule.condition();
    argument << rule.action();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, Rule &rule)
{
    QString   ruleId = rule.getRuleId();
    QString   ruleName = rule.getRuleName();
    bool      ruleActive = rule.getRuleActive();

    argument.beginStructure();
    argument >> ruleId;
    argument >> ruleName;
    argument >> ruleActive;
    argument >> rule.condition();
    argument >> rule.action();
    argument.endStructure();

    rule.setRuleId(ruleId);
    rule.setRuleName(ruleName);
    rule.setRuleActive(ruleActive);

    return argument;
}
