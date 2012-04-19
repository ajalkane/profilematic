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
#include "profilematicclient.h"

#define PM_PATH "/org/ajalkane/profilematic"
#define PM_SERVICE "org.ajalkane.profilematic"
#define PM_INTERFACE "org.ajalkane.profilematic"

ProfileMaticClient::ProfileMaticClient(QObject *parent) :
    QObject(parent), dbus_iface(NULL)
{
    qDBusRegisterMetaType<Rule>();
    qDBusRegisterMetaType<QList<Rule> >();
    qDBusRegisterMetaType<QStringList>();
    qDBusRegisterMetaType<PresenceRule>();

    dbus_iface = new QDBusInterface(PM_SERVICE, PM_PATH,
                                    PM_INTERFACE);
    connect(dbus_iface, SIGNAL(activeRuleIdsChanged(const QStringList &)), this, SIGNAL(activeRuleIdsChanged(const QStringList &)));

    connect(dbus_iface, SIGNAL(ruleUpdated(const Rule &)), this, SIGNAL(ruleUpdated(const Rule &)));
    connect(dbus_iface, SIGNAL(ruleAppended(const Rule &)), this, SIGNAL(ruleAppended(const Rule &)));
    connect(dbus_iface, SIGNAL(ruleRemoved(const QString &)), this, SIGNAL(ruleRemoved(const QString &)));
    connect(dbus_iface, SIGNAL(ruleMoved(const QString &, int)), this, SIGNAL(ruleMoved(const QString &, int)));
    connect(dbus_iface, SIGNAL(activeChanged(bool)), this, SIGNAL(activeChanged(bool)));
}

ProfileMaticClient::~ProfileMaticClient() {
    delete dbus_iface;
}

int
ProfileMaticClient::lastError() const {
    return dbus_iface->lastError().type();
}

QList<Rule>
ProfileMaticClient::getRules() const {
    QDBusReply<QList<Rule> > reply = dbus_iface->call("getRules");
    if (reply.isValid()) {
        QList<Rule> rules = reply.value();
        for (QList<Rule>::const_iterator i = rules.constBegin(); i != rules.constEnd(); ++i) {
            qDebug("ProfileMaticClient: rule id '%s', rule name %s",
                   qPrintable(i->getRuleId()), qPrintable(i->getRuleName()));
        }
        return rules;
    } else {
        QDBusError e = reply.error();
        qDebug("getRules error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }
    return QList<Rule>();
}

QStringList
ProfileMaticClient::getActiveRuleIds() const {
    QDBusReply<QStringList> reply = dbus_iface->call("getActiveRuleIds");
    if (reply.isValid()) {
        QStringList ruleIds = reply.value();
        return ruleIds;
    } else {
        QDBusError e = reply.error();
        qDebug("getActiveRuleRuleIds error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }
    return QStringList();
}

void
ProfileMaticClient::updateRule(const Rule &rule) {
    /*QDBusReply<QList<Rule> > reply = */ dbus_iface->call("updateRule", QVariant::fromValue(rule));
    /*if (!reply.isValid()) {
        QDBusError e = reply.error();
        qDebug("updateRule error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }*/
}

QString
ProfileMaticClient::appendRule(const Rule &rule) {
    QDBusReply<QString> reply = dbus_iface->call("appendRule", QVariant::fromValue(rule));
    if (!reply.isValid()) {
        QDBusError e = reply.error();
        qDebug("appendRule error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }
    return reply.value();
}

void
ProfileMaticClient::removeRule(const QString &ruleId) {
    /*QDBusReply<QList<Rule> > reply = */ dbus_iface->call("removeRule", QVariant::fromValue(ruleId));
    /*if (!reply.isValid()) {
        QDBusError e = reply.error();
        qDebug("updateRule error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }*/
}

void
ProfileMaticClient::moveRule(const QString &ruleId, int toIndex) {
    /*QDBusReply<QList<Rule> > reply = */ dbus_iface->call("moveRule", QVariant::fromValue(ruleId), toIndex);
    /*if (!reply.isValid()) {
        QDBusError e = reply.error();
        qDebug("updateRule error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }*/
}

bool
ProfileMaticClient::isActive() const {
    QDBusReply<bool> reply = dbus_iface->call("isActive");
    if (!reply.isValid()) {
        QDBusError e = reply.error();
        qDebug("isActive error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }
    return reply.value();
}


void
ProfileMaticClient::setActive(bool isActive) {
    /*QDBusReply<QList<Rule> > reply = */ dbus_iface->call("setActive", QVariant::fromValue(isActive));
    /*if (!reply.isValid()) {
        QDBusError e = reply.error();
        qDebug("updateRule error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }*/
}

bool
ProfileMaticClient::hasDeviceModeCredential() const {
    QDBusReply<bool> reply = dbus_iface->call("hasDeviceModeCredential");
    if (!reply.isValid()) {
        QDBusError e = reply.error();
        qDebug("hasDeviceModeCredential error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }
    return reply.value();
}

void
ProfileMaticClient::testCommandLine(const QString &commandLine) {
    /*QDBusReply<QList<Rule> > reply = */ dbus_iface->call("runCommandLine", QVariant::fromValue(commandLine));
    /*if (!reply.isValid()) {
        QDBusError e = reply.error();
        qDebug("updateRule error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }*/
}
