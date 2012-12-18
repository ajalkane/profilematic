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

#include <QtTest/QTest>
#include "testconditionmanagertime.h"
#include "signalcounter.h"

#include "../../src/logic/conditionmanagercaching.h"

TestConditionManagerTime::TestConditionManagerTime()
{
    for (int i = 0; i < 7; ++i) {
        _allDays.insert(i);
    }
}

// IMPROVE doesn't need to be class function
//const Rule *
//TestConditionManagerTime::_refresh(ConditionManagerTime &cm, const QList<Rule> &rules) {
//    ConditionManagerCaching cmcaching(&cm);

//    cmcaching.startRefresh();
//    QList<Rule>::const_iterator firstMatchingRule = rules.constBegin();
//    for (; firstMatchingRule != rules.constEnd(); ++firstMatchingRule) {
//        bool isMatching = cmcaching.refresh(firstMatchingRule->getRuleId(), firstMatchingRule->condition());
//        if (isMatching) {
//            break;
//        }
//    }
//    return firstMatchingRule != rules.constEnd() ? &(*firstMatchingRule) : 0;
//}

// IMPROVE doesn't need to be class function
const Rule *
TestConditionManagerTime::_refresh(ConditionManagerTime &cm, const QList<Rule> &rules, const QDateTime &now) {
//     ConditionManagerCaching cmcaching(&cm);

//    cmcaching.startRefresh(now);
    QList<Rule>::const_iterator firstMatchingRule = rules.constBegin();
    for (; firstMatchingRule != rules.constEnd(); ++firstMatchingRule) {
        ConditionManagerCacheable::MatchStatus match = cm.match(now, firstMatchingRule->condition());
        if (match != ConditionManagerCacheable::NotMatched) {
            break;
        }
    }
//     cmcaching.endRefresh();
    return firstMatchingRule != rules.constEnd() ? &(*firstMatchingRule) : 0;
}

void
TestConditionManagerTime::refresh_basicTestsTimer() {
    QList<Rule> rules;
    const Rule *match;
    Rule rule1;
    ConditionManagerTime cm;

    // Initial state
    QCOMPARE(cm.timer()->isActive(), false);

    // Empty rules
    match = _refresh(cm, rules, QDateTime::currentDateTime());

    QCOMPARE(cm.timer()->isActive(), false);
    QVERIFY(match == 0);

    // Check that if unusable day, startTime and endTime, not used, but matching
    rules.clear();
    rule1 = Rule();
    rules << rule1;
    match = _refresh(cm, rules, QDateTime::currentDateTime());
    QCOMPARE(cm.timer()->isActive(), false);
    QVERIFY(*match == rule1);

    // Match to next start day
    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    rule1.condition().setDays(_allDays);
    rule1.condition().setTimeStart(now.addSecs(100).time());
    rule1.condition().setTimeEnd(now.addSecs(200).time());
    rules[0] = rule1;
    match = _refresh(cm, rules, now);
    QCOMPARE((int)cm.timer()->lastError(), 0);
    QVERIFY(cm.timer()->isActive() == true);
    QVERIFY(match == 0);
    // 100 = 1min40secs, start time is actually minute after
    QCOMPARE(cm.minimumIntervalSec(), 60);

    // Match to next day
    cm.rulesChanged();
    rule1.condition().setDays(_allDays);
    rule1.condition().setTimeStart(now.addSecs(-100).time());
    rule1.condition().setTimeEnd(now.addSecs(-50).time());
    rules[0] = rule1;
    match = _refresh(cm, rules, now);
    QVERIFY(cm.timer()->isActive() == true);
    QVERIFY(match == 0);
    // 100 = 1min40secs, start time is actually 2 minutes before
    QCOMPARE(cm.minimumIntervalSec(), (24 * 60 * 60 - 120));
}

void
TestConditionManagerTime::refresh_basicTestsMatching() {
    QList<Rule> rules;
    const Rule *match;
    QSet<int> days;
    Rule rule1;
    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    ConditionManagerTime cm;

    // Match to next end day
    rule1.condition().setDays(_allDays);
    rule1.condition().setTimeStart(now.addSecs(-100).time());
    rule1.condition().setTimeEnd(now.addSecs(100).time());
    rules << rule1;
    match = _refresh(cm, rules, now);
    QCOMPARE(cm.timer()->isActive(), true);
    QVERIFY(*match == rule1);
    // 100 = 1min40secs, end time is actually 1min after
    QCOMPARE(cm.minimumIntervalSec(), 60);

    // Match to next day, when start time is current time, and end time is current time - minute (21:59:00), even as only tuesday selected
    // Tuesday
    cm.rulesChanged();
    now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    rule1 = Rule();
    days.clear();
    rules.clear();
    // Tuesday
    days.insert(1);
    rule1.condition().setDays(days);
    rule1.condition().setTimeStart(now.time());
    rule1.condition().setTimeEnd(now.addSecs(-60).time());
    rules << rule1;
    match = _refresh(cm, rules, now);
    QCOMPARE(cm.timer()->isActive(), true);
    QVERIFY(*match == rule1);
    // 24h - 60 seconds
    QCOMPARE(cm.minimumIntervalSec(), (24 * 60 * 60 - 60) );


    // Match to endTime, when startTime is in the previous day
    // Wednesday, but startTime is in tuesday
    now = QDateTime::fromString("28.09.2011 06:00", "dd.MM.yyyy hh:mm");
    days.clear();
    rules.clear();
    days.insert(1);
    rule1.condition().setDays(days);
    // startTime is on tuesday, 22:00
    rule1.condition().setTimeStart(now.addSecs(-8 * 60 * 60).time());
    rule1.condition().setTimeEnd(now.addSecs(60).time());
    rules << rule1;
    match = _refresh(cm, rules, now);
    QCOMPARE((int)cm.timer()->lastError(), 0);
    QVERIFY(cm.timer()->isActive() == true);
    QVERIFY(*match == rule1);
    QCOMPARE(cm.minimumIntervalSec(), 60);

    // Match to next day when startTime = endTime and now = startTime
    // Tuesday
    cm.rulesChanged();
    now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    rule1 = Rule();
    days.clear();
    rules.clear();
    // Tuesday
    days.insert(1);
    rule1.condition().setDays(days);
    rule1.condition().setTimeStart(now.time());
    rule1.condition().setTimeEnd(now.time());
    rules << rule1;
    match = _refresh(cm, rules, now);
    QCOMPARE(cm.timer()->isActive(), true);
    QVERIFY(match != 0);
    QVERIFY(*match == rule1);
    // 24h
    QCOMPARE(cm.minimumIntervalSec(), 24 * 60 * 60 );
}

void
TestConditionManagerTime::refresh_dayTimeTests() {
    QList<Rule> rules;
    const Rule *match;
    QSet<int> days;
    Rule rule1;
    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    ConditionManagerTime cm;

    // Check a rule with one day selected, with time 1 minute after now
    days << 1; // Tuesday
    rule1.condition().setDays(days);
    rule1.condition().setTimeStart(now.time().addSecs(60));
    rule1.condition().setTimeEnd(now.time().addSecs(60));
    rules.clear();
    rules << rule1;

    match = _refresh(cm, rules, now);
    QVERIFY(cm.timer()->isActive() == true);
    QVERIFY(match == 0);
    QCOMPARE(cm.minimumIntervalSec(), 60 );

    // Then try with a time that is a 2 minute before. In that case wakeup should go to next week.
    cm.rulesChanged();
    rules.first().condition().setTimeStart(now.time().addSecs(-120));
    rules.first().condition().setTimeEnd(now.time().addSecs(-60));
    match = _refresh(cm, rules, now);
    QVERIFY(match == 0);
    QCOMPARE(cm.minimumIntervalSec(), (7 * 24 * 60 * 60 - 2*60) );
}

void
TestConditionManagerTime::refresh_multiRuleTests() {
    QList<Rule> rules;
    const Rule *match;
    Rule rule1;
    Rule rule2;

    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    ConditionManagerTime cm;

    // Check that if rules equal, the first one matching (in order) is used
    rule1.condition().setDays(_allDays);
    rule1.condition().setTimeStart(now.addSecs(120).time());
    rule1.condition().setTimeEnd(now.addSecs(180).time());
    rule2.condition().setDays(_allDays);
    rule2.condition().setTimeStart(rule1.condition().getTimeStart());
    rule2.condition().setTimeEnd(rule1.condition().getTimeEnd());

    rules << rule1 << rule2;

    match = _refresh(cm, rules, now);
    QVERIFY(cm.timer()->isActive() == true);
    QVERIFY(match == 0);
    QCOMPARE(cm.minimumIntervalSec(), 120 );
}

void
TestConditionManagerTime::refresh_multiRuleTestsMatching() {
    QList<Rule> rules;
    const Rule *match;
    Rule rule1;
    Rule rule2;

    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    ConditionManagerTime cm;

    // Check that if several rules fall into same time window, the first one matches.
    // IMPROVE this kind of test belongs to RulesManager unit tests, which don't exist yet
    rule1.condition().setDays(_allDays);
    rule1.condition().setTimeStart(now.addSecs(-60).time());
    rule1.condition().setTimeEnd(now.addSecs(60).time());
    rule2.condition().setDays(_allDays);
    rule2.condition().setTimeStart(rule1.condition().getTimeStart());
    rule2.condition().setTimeEnd(rule1.condition().getTimeEnd());

    rules << rule1 << rule2;

    match = _refresh(cm, rules, now);
    QCOMPARE(cm.timer()->isActive(), true);
    QVERIFY(*match == rule1);
    QCOMPARE(cm.minimumIntervalSec(), 60 );
}

void
TestConditionManagerTime::refresh_nextIntervalRoundedUp() {
    QList<Rule> rules;
    const Rule *match;
    Rule rule1;
    ConditionManagerTime cm;

    // Initial state
    // Tuesday, with one millisecond over 10 o'clock
    qDebug("constructing now");
    QDateTime now = QDateTime::fromString("27.09.2011 10:00:00:1", "dd.MM.yyyy hh:mm:ss:z");
    qDebug("constructing done");
    rule1.condition().setDays(_allDays);
    rule1.condition().setTimeStart(now.addSecs(60).time());
    rule1.condition().setTimeEnd(now.addSecs(120).time());
    rules << rule1;
    match = _refresh(cm, rules, now);
    QCOMPARE((int)cm.timer()->lastError(), 0);
    QVERIFY(cm.timer()->isActive() == true);
    QVERIFY(match == 0);
    // Exact interval is 60 seconds - 1msec, but since the underlying timer operates on second boundaries,
    // we should have 60 interval
    QCOMPARE(cm.minimumIntervalSec(), 60 );
}

void
TestConditionManagerTime::refreshNeeded_signalTest() {
    QList<Rule> rules;
    const Rule *match;
    Rule rule;
    SignalCounter signalTarget;
    QSet<int> days;

    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 09:59:59", "dd.MM.yyyy hh:mm:ss");
    ConditionManagerTime cm;

    days << 1;
    rule.condition().setDays(days);
    rule.condition().setTimeStart(now.addSecs(200).time());
    rule.condition().setTimeEnd(now.addSecs(200).time());

    rules << rule;

    // A following rules has earlier timeout time. This makes sure
    // that ConditionManagerTime's timer implementation handles calling
    // of new .start(min, max) correctly
    rule.condition().setDays(days);
    rule.condition().setTimeStart(now.addSecs(1).time());
    rule.condition().setTimeEnd(now.addSecs(1).time());

    rules << rule;

    QCOMPARE(signalTarget.numSignal, 0);
    QVERIFY(cm.timer()->isActive() == false);

    connect(&cm, SIGNAL(matchInvalidated()), &signalTarget, SLOT(onSignal()));

    cm.setTimerMaxIntervalAddition(1);
    match = _refresh(cm, rules, now);

    QCOMPARE(signalTarget.numSignal, 0);
    QVERIFY(match == 0);
    QCOMPARE(cm.timer()->isActive(), true);
    QCOMPARE(cm.minimumIntervalSec(), 1);

    // Wait for 5 seconds, the signal should be fired by then.
    qDebug("Waiting 5 * 1000 msec");
    QTest::qWait(5 * 1000);

    QCOMPARE(signalTarget.numSignal, 1);
}

void
TestConditionManagerTime::timeoutBeforeShouldBeRescheduledCorrectly() {
    QList<Rule> rules;
    const Rule *match;
    Rule rule;
    SignalCounter signalTarget;

    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 10:00:00", "dd.MM.yyyy hh:mm:ss");
    ConditionManagerTime cm;

    rule.condition().setDays(_allDays);
    rule.condition().setTimeStart(now.addSecs(60).time());
    rule.condition().setTimeEnd(now.addSecs(120).time());

    rules << rule;

    QVERIFY(cm.timer()->isActive() == false);

    connect(&cm, SIGNAL(matchInvalidated()), &signalTarget, SLOT(onSignal()));

    cm.setTimerMaxIntervalAddition(1);
    match = _refresh(cm, rules, now);

    cm.timer()->start(1, 1);
    QTest::qWait(5 * 1000);
    cm.setTimerMaxIntervalAddition(1);
    match = _refresh(cm, rules, now);

    QCOMPARE(signalTarget.numSignal, 1);
    QVERIFY(match == false);
    QCOMPARE(cm.timer()->isActive(), true);
    QCOMPARE(cm.minimumIntervalSec(), 60);
}

// IMPROVE: exceptional cases, no days, no startTime, no endTime
