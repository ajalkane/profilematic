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
#ifndef TESTRULEWATCH_H
#define TESTRULEWATCH_H

#include <QtTest/QtTest>

#include "../../src/model/rule.h"

class RuleWatchSignalTarget : public QObject {
    Q_OBJECT

public:
    int numRuleActivated;
    const Rule *ruleActivated;

    RuleWatchSignalTarget();

public slots:
    void onRuleActivated(const Rule &ruleItem);
};

class TestRuleWatch : public QObject
{
    Q_OBJECT

private slots:
    void refreshWatch_basicTests();
    void refreshWatch_dayTests();
    void refreshWatch_dayTimeTests();
    void refreshWatch_multiRuleTests();

    void activateRule_signalTest();
};

#endif // TESTRULEWATCH_H
