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
#ifndef PROFILEMATICCLIENT_H
#define PROFILEMATICCLIENT_H

#include <QString>
#include <QObject>
#include <QList>
#include <QtDBus/QtDBus>

#include "model/rule.h"

class ProfileMaticClient : public QObject
{
    Q_OBJECT

    QDBusInterface *dbus_iface;
public:
    explicit ProfileMaticClient(QObject *parent = 0);
    ~ProfileMaticClient();

    QList<Rule> getRules() const;
    QStringList getMatchingRuleIds() const;
    void updateRule(const Rule &rule);
    QString appendRule(const Rule &rule);
    void removeRule(const QString &ruleId);
    void moveRule(const QString &ruleId, int toIndex);

    bool isActive() const;
    void setActive(bool isActive);

    bool hasDeviceModeCredential() const;

    void testCommandLine(const QString &commandLine);
    void executeAction(const RuleAction &action) const;

    // Returns QDBusError::ErrorType of last call. 0 is no error.
    int lastError() const;
signals:
    void ruleUpdated(const Rule &rule);
    void ruleAppended(const Rule &rule);
    void ruleRemoved(const QString &ruleId);
    void ruleMoved(const QString &ruleId, int toIndex);
    void activeChanged(bool);

    void matchingRuleIdsChanged(const QStringList &matchingRuleIds);
public slots:
};

#endif // PROFILEMATICCLIENT_H
