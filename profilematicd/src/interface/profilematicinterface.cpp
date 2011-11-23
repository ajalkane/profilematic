#include <QtDBus/QtDBus>

#include <stdio.h>

#include "../model/rule.h"
#include "profilematicinterface.h"

#define PM_OBJECT_NAME "/org/ajalkane/profilematic"
#define PM_SERVICE_NAME "org.ajalkane.profilematic"

ProfileMaticInterface::ProfileMaticInterface(QList<Rule> *rules, QObject *parent) :
    QObject(parent), _rules(rules)
{
    qDBusRegisterMetaType<Rule>();
    qDBusRegisterMetaType<QList<Rule> >();
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

void
ProfileMaticInterface::updateRule(const Rule &rule) {
    qDebug("updateRule received %s", qPrintable(rule.getRuleId()));
    int index = _findRuleIndexById(rule.getRuleId());
    if (index < 0) {
        qDebug("Could not find rule with id %s", qPrintable(rule.getRuleId()));
        return;
    }

    (*_rules)[index] = rule; // Rule(rule);
    qDebug("updateRule emitting ruleUpdated for %s", qPrintable(rule.getRuleId()));
    emit ruleUpdated(_rules->at(index));
}

QString
ProfileMaticInterface::appendRule(const Rule &rule) {
    Rule newRule = rule;
    newRule.setRuleId(QUuid::createUuid().toString());
    qDebug("ProfileMaticInterface::createRule(), created ruleId %s", qPrintable(newRule.getRuleId()));

    _rules->append(newRule);
    emit ruleAppended(newRule);
    return newRule.getRuleId();
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
