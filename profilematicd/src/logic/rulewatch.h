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
#ifndef RULEWATCH_H
#define RULEWATCH_H

#include <QObject>
#include <QDateTime>
#include <QTimer>

#include "../model/rule.h"

class RuleWatch : public QObject
{
    Q_OBJECT

    QTimer _timer;
    const QList<Rule> *_rules;
    const Rule *_targetRule;

    QDateTime _nextDateTimeFromRule(const QDateTime &from, const Rule &rule) const;
    bool _isTargetRuleActivable() const;

public:
    RuleWatch(const QList<Rule> *_rules, QObject *parent = 0);

    void refreshWatch(const QDateTime &now);
    void refreshWatch();

    // These accessor functions are pretty much only needed for unit tests
    const QTimer *timer() const;
    const Rule *targetRule() const;
    const QList<Rule> *rules() const;
private slots:
    void _activateAndReschedule();
signals:
    void activateRule(const Rule &rule);
};

#endif // RULEWATCH_H
