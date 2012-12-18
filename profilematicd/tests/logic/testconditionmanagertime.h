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
#ifndef TESTCONDITIONMANAGERTIME_H
#define TESTCONDITIONMANAGERTIME_H

#include <QList>
#include <QSet>
#include <QtTest/QtTest>

#include "../../src/logic/conditionmanagertime.h"
#include "../../src/model/rule.h"

class TestConditionManagerTime : public QObject
{
    Q_OBJECT

    // const Rule * _refresh(ConditionManagerTime &cm, const QList<Rule> &rules);
    const Rule * _refresh(ConditionManagerTime &cm, const QList<Rule> &rules, const QDateTime &now);

    QSet<int> _allDays;
public:
    TestConditionManagerTime();

private slots:
    void refresh_basicTestsTimer();
    void refresh_basicTestsMatching();
    void refresh_dayTimeTests();
    void refresh_multiRuleTests();
    void refresh_multiRuleTestsMatching();
    void refresh_nextIntervalRoundedUp();
    void refreshNeeded_signalTest();

    void timeoutBeforeShouldBeRescheduledCorrectly();

};

#endif // TESTCONDITIONMANAGERTIME_H
