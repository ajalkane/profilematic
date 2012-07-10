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
#include <QtDBus/QtDBus>

#include <stdio.h>

#include "../model/rule.h"
#include "../model/presencerule.h"
#include "../configuration.h"
#include "../logic/actioncommandline.h"
#include "../platform/platformutil.h"

#include "profilematicinterface.h"

#define PM_OBJECT_NAME "/org/ajalkane/profilematic"
#define PM_SERVICE_NAME "org.ajalkane.profilematic"

ProfileMaticInterface::ProfileMaticInterface(RulesManager *rulesManager, QList<Rule> *rules, Preferences *preferences, QObject *parent) :
    QObject(parent), _rulesManager(rulesManager), _rules(rules), _preferences(preferences)
{
    qDBusRegisterMetaType<Rule>();
    qDBusRegisterMetaType<QList<Rule> >();
    qDBusRegisterMetaType<QStringList>();
    qDBusRegisterMetaType<PresenceRule>();

    connect(_rulesManager, SIGNAL(matchingRuleIdsChanged(QStringList)), this, SIGNAL(matchingRuleIdsChanged(QStringList)));
}

int
ProfileMaticInterface::init() {
    QDBusConnection bus = QDBusConnection::sessionBus();
    if (!bus.registerObject(PM_OBJECT_NAME, this, QDBusConnection::ExportAllSlots|QDBusConnection::ExportAllSignals)) {
        fprintf(stderr, "Could not register D-Bus object at %s. Daemon already running?\n", PM_OBJECT_NAME);
        return -1;
    }

    if (!bus.registerService(PM_SERVICE_NAME)) {
        fprintf(stderr, "Could not register D-Bus service at %s. Daemon already running?\n", PM_SERVICE_NAME);
        return -1;
    }

    return 0;
}

ProfileMaticInterface::~ProfileMaticInterface() {
    QDBusConnection bus = QDBusConnection::sessionBus();
    bus.unregisterService(PM_SERVICE_NAME);
    bus.unregisterObject(PM_OBJECT_NAME);
}

QList<Rule>
ProfileMaticInterface::getRules() const {
    return *_rules;
}


// QList<QString>
QStringList
ProfileMaticInterface::getMatchingRuleIds() const {
    return _rulesManager->matchingRuleIds().toList();
}

void
ProfileMaticInterface::updateRule(const Rule &rule) {
    qDebug("updateRule received %s", qPrintable(rule.getRuleId()));
    int index = _findRuleIndexById(rule.getRuleId());
    if (index < 0) {
        qDebug("Could not find rule with id %s", qPrintable(rule.getRuleId()));
        return;
    }

    if (!rule.isDefaultRule()) {
        (*_rules)[index] = rule; // Rule(rule);
    } else {
        qDebug("updateRule was defaultRule, only actions updated");
        (*_rules)[index].actionsFrom(rule);
    }
    qDebug("updateRule emitting ruleUpdated for %s", qPrintable(rule.getRuleId()));
    emit ruleUpdated(_rules->at(index));
    _rulesChanged();
}

QString
ProfileMaticInterface::appendRule(const Rule &rule) {
    if (rule.isDefaultRule()) {
        qDebug("ProfileMaticInterface::appendRule(): client->appendRule can not append default rule");
        return QString();
    }
    Rule newRule = rule;
    newRule.setRuleId(QUuid::createUuid().toString());
    if (newRule.getRuleName().isEmpty()) {
        QString ruleName = QString("Rule #") + QString::number(_rules->size() + 1);
        newRule.setRuleName(ruleName);
        qDebug("ProfileMaticInterface::appendRule(): setting name to %s", qPrintable(newRule.getRuleName()));

    }
    qDebug("ProfileMaticInterface::appendRule(), created ruleId %s", qPrintable(newRule.getRuleId()));
    // Size() - 1 because expecting defaultRule to always be the last rule
    _rules->insert(_rules->size() - 1, newRule);
    emit ruleAppended(newRule);
    _rulesChanged();
    return newRule.getRuleId();
}

void
ProfileMaticInterface::removeRule(const QString &ruleId) {
    qDebug("removeRule received %s", qPrintable(ruleId));
    if (ruleId == DEFAULT_RULE_ID) {
        qDebug("ProfileMaticInterface::removeRule(): can not remove default rule");
        return;
    }

    int index = _findRuleIndexById(ruleId);
    if (index < 0) {
        qDebug("removeRule: Could not find rule with id %s", qPrintable(ruleId));
        return;
    }

    _rules->removeAt(index);
    _rulesChanged();
    emit ruleRemoved(ruleId);
}

void
ProfileMaticInterface::moveRule(const QString &ruleId, int toIndex) {
    qDebug("moveRule received %s -> %d", qPrintable(ruleId), toIndex);
    if (ruleId == DEFAULT_RULE_ID) {
        qDebug("ProfileMaticInterface::moveRule(): can not move default rule");
        return;
    }
    int index = _findRuleIndexById(ruleId);
    if (index < 0) {
        qDebug("moveRule: Could not find rule with id %s", qPrintable(ruleId));
        return;
    }
    // Assume defaultRule is the last index always, so upper limit size() - 2
    else if (toIndex < 0 || toIndex > _rules->size() - 2) {
        qDebug("moveRule: invalid toIndex: %d, allowed range (0 - %d)", toIndex, _rules->size() - 1);
        return;
    }
    // Assume defaultRule is the last index always
    if (index == _rules->size() - 1) {
        qDebug("moveRule: can not move defaultRule from last place");
        return;
    }
    _rules->move(index, toIndex);
    _rulesChanged();
    emit ruleMoved(ruleId, toIndex);
}

bool
ProfileMaticInterface::isActive() const {
    return _preferences->isActive;
}

void
ProfileMaticInterface::setActive(bool isActive) {
    if (isActive != _preferences->isActive) {
        qDebug("Active changed to %d", isActive);
          _preferences->isActive = isActive;
        emit activeChanged(isActive);
        _rulesManager->refreshRules();
        _preferencesChanged();
    }
}

bool
ProfileMaticInterface::hasDeviceModeCredential() const {
    return PlatformUtil::instance()->hasDeviceModeCredential();
}

int
ProfileMaticInterface::_findRuleIndexById(const Rule::IdType &id) const {
    for (int i = 0; i < _rules->size(); ++i) {
        const Rule &r = _rules->at(i);
        if (id == r.getRuleId()) {
            return i;
        }
    }
    return -1;
}

void
ProfileMaticInterface::_rulesChanged() {
    _rulesManager->refreshRules();
    Configuration::writeRules(*_rules);
}

void
ProfileMaticInterface::_preferencesChanged() {
    Configuration::writePreferences(*_preferences);
}

void
ProfileMaticInterface::runCommandLine(const QString &commandLine) const {
    RuleAction ruleAction;
    ruleAction.setCommandLine(commandLine);
    ActionCommandLine action;
    action.activate(QString(), ruleAction);
}

//// Returns id of created rule or empty if error. A new rule is always
//// added as last rule.
//QString
//ProfileMaticInterface::addRule(const QString &id);

//// Returns true if success
//bool
//ProfileMaticInterface::deleteRule(const QString &id);

//// Moves rule to position given by index. The valid
//// values are in the range 0 - amount of rules.
//// Returns true if success.
//int
//ProfileMaticInterface::moveRule(const QString &id, int index);

//// Setters and getters
//QString
//ProfileMaticInterface::getRuleName(const QString &id);

//bool
//ProfileMaticInterface::setRuleName(const QString &id, const QString &name);
