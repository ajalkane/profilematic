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
#ifndef PROFILEMATICINTERFACE_H
#define PROFILEMATICINTERFACE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>

#include "../model/rule.h"
#include "../configuration.h"
#include "../preferences.h"
#include "../logic/rulewatch.h"

class ProfileMaticInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ajalkane.profilematic")
    RuleWatch   *_ruleWatch;
    QList<Rule> *_rules;
    Preferences *_preferences;

public:
    explicit ProfileMaticInterface(RuleWatch *ruleWatch, QList<Rule> *rules, Preferences *preferences, QObject *parent = 0);
    virtual ~ProfileMaticInterface();

    // Return 0 if success
    int init();
signals:
    void ruleUpdated(const Rule &rule);
    // Rule is always created as the last rule
    void ruleAppended(const Rule &rule);
    void ruleRemoved(const QString &ruleId);
    void ruleMoved(const QString &ruleId, int toIndex);
    void activeChanged(bool isActive);
public slots:
    QList<Rule> getRules() const;

    void updateRule(const Rule &rule);
    // Creates a new rule. Returns the new ruleId. If the given rule
    // already has a ruleId, it is not used.
    QString appendRule(const Rule &rule);
    void removeRule(const QString &ruleId);
    void moveRule(const QString &ruleId, int index);

    // Returns true if rules are watched
    bool isActive() const;
    void setActive(bool active);

private:
    int _findRuleIndexById(const Rule::IdType &id) const;
    void _rulesChanged();
    void _preferencesChanged();
};

#endif // PROFILEMATICINTERFACE_H
