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

TestConditionManagerTime::TestConditionManagerTime()
{
    for (int i = 0; i < 7; ++i) {
        _allDays.insert(i);
    }
}

QList<Rule>
TestConditionManagerTime::_refresh(ConditionManagerTime &cm, const QList<Rule> &rules) {
    cm.startRefresh();
    cm.refresh(rules);
    QList<Rule> matchingRules = cm.matchingRules();
    cm.endRefresh();
    return matchingRules;
}

QList<Rule>
TestConditionManagerTime::_refresh(ConditionManagerTime &cm, const QList<Rule> &rules, const QDateTime &now) {
    cm.startRefresh();
    cm.refresh(rules, now);
    QList<Rule> matchingRules = cm.matchingRules();
    cm.endRefresh();
    return matchingRules;
}

void
TestConditionManagerTime::refresh_basicTestsTimer() {
    QList<Rule> rules;
    QList<Rule> matches;
    QList<Rule> expect;
    Rule rule1;
    ConditionManagerTime cm;

    // Initial state
    QCOMPARE(cm.timer()->isActive(), false);

    // Empty rules
    matches = _refresh(cm, rules);

    QCOMPARE(cm.timer()->isActive(), false);
    QVERIFY(matches.isEmpty());

    // TODO this is not basic test, but exceptional use case
    // Check that if unusable day, not used, and not matching
    rule1.setTimeStart(QTime::currentTime());
    rule1.setTimeEnd(QTime::currentTime());
    rules << rule1;
    matches = _refresh(cm, rules);
    QCOMPARE(cm.timer()->isActive(), false);
    QVERIFY(matches.isEmpty());

    // Check that if unusable day, startTime and endTime, not used, but matching
    rules.clear();
    rule1 = Rule();
    rules << rule1;
    matches = _refresh(cm, rules);
    QCOMPARE(cm.timer()->isActive(), false);
    QCOMPARE(matches, rules);

    // Match to next start day
    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    rule1.setDays(_allDays);
    rule1.setTimeStart(now.addSecs(100).time());
    rule1.setTimeEnd(now.addSecs(200).time());
    rules[0] = rule1;
    expect = QList<Rule>();
    matches = _refresh(cm, rules, now);
    QVERIFY(cm.timer()->isActive() == true);
    QCOMPARE(expect, matches);
    // 100 = 1min40secs, start time is actually minute after
    QCOMPARE(cm.timer()->interval(), 60 * 1000);

    // Match to next day
    rule1.setDays(_allDays);
    rule1.setTimeStart(now.addSecs(-100).time());
    rule1.setTimeEnd(now.addSecs(-50).time());
    rules[0] = rule1;
    matches = _refresh(cm, rules, now);
    QVERIFY(cm.timer()->isActive() == true);
    // 100 = 1min40secs, start time is actually 2 minutes before
    QCOMPARE(cm.timer()->interval(), (24 * 60 * 60 - 120) * 1000);
}

void
TestConditionManagerTime::refresh_basicTestsMatching() {
    QList<Rule> rules;
    QList<Rule> matches;
    QList<Rule> expect;
    QSet<int> days;
    Rule rule1;
    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    ConditionManagerTime cm;

    // Match to next end day
    rule1.setDays(_allDays);
    rule1.setTimeStart(now.addSecs(-100).time());
    rule1.setTimeEnd(now.addSecs(100).time());
    rules << rule1;
    expect = QList<Rule>();
    expect << rule1;
    matches = _refresh(cm, rules, now);
    QCOMPARE(cm.timer()->isActive(), true);
    QCOMPARE(expect, matches);
    // 100 = 1min40secs, end time is actually 1min59s after
    QCOMPARE(cm.timer()->interval(), (60 + 59) * 1000);

    // Match to next day, when start time is current time, and end time is current time - minute (21:59:59), even as only tuesday selected
    // Tuesday
    now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    rule1 = Rule();
    days.clear();
    rules.clear();
    // Tuesday
    days.insert(1);
    rule1.setDays(days);
    rule1.setTimeStart(now.time());
    rule1.setTimeEnd(now.addSecs(-60).time());
    rules << rule1;
    expect = QList<Rule>();
    expect << rule1;
    matches = _refresh(cm, rules, now);
    QCOMPARE(cm.timer()->isActive(), true);
    QCOMPARE(expect, matches);
    // 24h - 1 seconds
    QCOMPARE(cm.timer()->interval(), (24 * 60 * 60 - 1) * 1000);

    // Match to :59 secs when startTime = endTime and now = startTime
    // Tuesday
    now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    rule1 = Rule();
    days.clear();
    rules.clear();
    // Tuesday
    days.insert(1);
    rule1.setDays(days);
    rule1.setTimeStart(now.time());
    rule1.setTimeEnd(now.time());
    rules << rule1;
    expect = QList<Rule>();
    expect << rule1;
    matches = _refresh(cm, rules, now);
    QCOMPARE(cm.timer()->isActive(), true);
    QCOMPARE(expect, matches);
    // 24h - 1 seconds
    QCOMPARE(cm.timer()->interval(), 59 * 1000);
}

void
TestConditionManagerTime::refresh_dayTimeTests() {
    QList<Rule> rules;
    QList<Rule> matches;
    QList<Rule> expect;
    QSet<int> days;
    Rule rule1;
    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    ConditionManagerTime cm;

    // Check a rule with one day selected, with time 1 minute after now
    days << 1; // Tuesday
    rule1.setDays(days);
    rule1.setTimeStart(now.time().addSecs(60));
    rule1.setTimeEnd(now.time().addSecs(60));
    rules.clear();
    rules << rule1;

    matches = _refresh(cm, rules, now);
    QVERIFY(cm.timer()->isActive() == true);
    expect.clear();
    QCOMPARE(expect, matches);
    QCOMPARE(cm.timer()->interval(), 60 * 1000);

    // Then try with a time that is a minute before. In that case wakeup should go to next week.
    rules.first().setTimeStart(now.time().addSecs(-60));
    rules.first().setTimeEnd(now.time().addSecs(-60));
    matches = _refresh(cm, rules, now);
    expect.clear();
    QCOMPARE(expect, matches);
    QCOMPARE(cm.timer()->interval(), (7 * 24 * 60 * 60 - 60) * 1000);
}

void
TestConditionManagerTime::refresh_multiRuleTests() {
    QList<Rule> rules;
    QList<Rule> matches;
    QList<Rule> expect;
    QSet<int> days;
    Rule rule1;
    Rule rule2;

    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    ConditionManagerTime cm;

    // Check that if rules equal, the first one matching (in order) is used
    rule1.setDays(_allDays);
    rule1.setTimeStart(now.addSecs(120).time());
    rule1.setTimeEnd(now.addSecs(120).time());
    rule2.setDays(_allDays);
    rule2.setTimeStart(rule2.getTimeStart());
    rule2.setTimeEnd(rule2.getTimeEnd());

    rules << rule1 << rule2;

    matches = _refresh(cm, rules, now);
    QVERIFY(cm.timer()->isActive() == true);
    expect.clear();
    QCOMPARE(expect, matches);
    QCOMPARE(cm.timer()->interval(), 120 * 1000);
}

void
TestConditionManagerTime::refresh_multiRuleTestsMatching() {
    QList<Rule> rules;
    QList<Rule> matches;
    QList<Rule> expect;
    Rule rule1;
    Rule rule2;

    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    ConditionManagerTime cm;

    // Check that if several rules fall into same time window, all are returned as matching.
    // And with the correct order.
    rule1.setDays(_allDays);
    rule1.setTimeStart(now.addSecs(-60).time());
    rule1.setTimeEnd(now.addSecs(60).time());
    rule2.setDays(_allDays);
    rule2.setTimeStart(rule1.getTimeStart());
    rule2.setTimeEnd(rule1.getTimeEnd());

    rules << rule1 << rule2;

    matches = _refresh(cm, rules, now);
    QVERIFY(cm.timer()->isActive() == true);
    expect.clear();
    expect << rule1 << rule2;
    QCOMPARE(expect, matches);
    QCOMPARE(cm.timer()->interval(), (60 + 59) * 1000);
}

// IMPROVE: exceptional cases, no days, no startTime, no endTime
