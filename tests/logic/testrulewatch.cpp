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

#include "testrulewatch.h"

#include "../../src/logic/rulewatch.h"
#include "../../src/model/rules.h"

RuleWatchSignalTarget::RuleWatchSignalTarget()
    : numRuleActivated(0), ruleActivated(NULL)
{

}

void
RuleWatchSignalTarget::onRuleActivated(const RuleItem &ruleItem)
{
    numRuleActivated++;
    ruleActivated = &ruleItem;
}

void
TestRuleWatch::refreshWatch_basicTests() {
    Rules rules;
    RuleWatch ruleWatch(&rules);

    // Initial state
    QVERIFY(ruleWatch.timer()->isActive() == false);
    QVERIFY(ruleWatch.targetRuleItem() == NULL);
    // Empty rules
    ruleWatch.refreshWatch();
    QVERIFY(ruleWatch.timer()->isActive() == false);
    QVERIFY(ruleWatch.targetRuleItem() == NULL);

    // Check that if unusable day/time, not used
    RuleItem item1;
    item1.ruleActive = true;
    rules << item1;
    ruleWatch.refreshWatch();
    QVERIFY(ruleWatch.timer()->isActive() == false);
    QVERIFY(ruleWatch.targetRuleItem() == NULL);

    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");
    item1.rule.setTimeStart(now.addSecs(100).time());
    rules[0] = item1;
    ruleWatch.refreshWatch(now);
    QVERIFY(ruleWatch.timer()->isActive() == true);
    QCOMPARE(*(ruleWatch.targetRuleItem()), item1);
    QCOMPARE(ruleWatch.timer()->interval(), 100 * 1000);

    // Match to next day
    item1.rule.setTimeStart(now.addSecs(-100).time());
    rules[0] = item1;
    ruleWatch.refreshWatch(now);
    QVERIFY(ruleWatch.timer()->isActive() == true);
    QCOMPARE(*(ruleWatch.targetRuleItem()), item1);
    QCOMPARE(ruleWatch.timer()->interval(), (24 * 60 * 60 - 100) * 1000);
}

void
TestRuleWatch::refreshWatch_dayTests() {
    Rules rules;
    RuleWatch ruleWatch(&rules);
    QSet<int> days; // 0 - 6 (mon - su)
    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");

    // Check a rule with one day selected, without time
    RuleItem item1;
    item1.ruleActive = true;
    days << 2; // Wednesday
    item1.rule.setDays(days);
    rules << item1;

    ruleWatch.refreshWatch(now);
    QVERIFY(ruleWatch.timer()->isActive() == true);
    // From 22:00 in tuesday, it should be 2 hours until wednesday
    QCOMPARE(ruleWatch.timer()->interval(), (2 * 60 * 60) * 1000);
    QCOMPARE(*(ruleWatch.targetRuleItem()), item1);

    // Tuesday
    days.clear();
    days << 1; // Tuesday
    item1.rule.setDays(days);
    rules[0] = item1;
    ruleWatch.refreshWatch(now);
    QVERIFY(ruleWatch.timer()->isActive() == true);
    // From 22:00 in tuesday, it should be 6 days and 2 hours until next tuesday
    QCOMPARE(ruleWatch.timer()->interval(), (6 * 24 * 60 * 60 + 2 * 60 * 60) * 1000);
    QCOMPARE(*(ruleWatch.targetRuleItem()), item1);

    // Check multiple days, matches the nearest
    days.clear();
    days << 1; // Tuesday
    days << 3; // Thursday
    item1.rule.setDays(days);
    rules[0] = item1;
    ruleWatch.refreshWatch(now);
    QVERIFY(ruleWatch.timer()->isActive() == true);
    // From 22:00 in tuesday, it should be 1 days and 2 hours until thursday
    QCOMPARE(ruleWatch.timer()->interval(), (1 * 24 * 60 * 60 + 2 * 60 * 60) * 1000);
    QCOMPARE(*(ruleWatch.targetRuleItem()), item1);
    // Same rules, but when now is Thursday 22:00
    ruleWatch.refreshWatch(now.addDays(2));
    QVERIFY(ruleWatch.timer()->isActive() == true);
    // From 22:00 in Thursday, it should be 4 days and 2 hours until Tuesday
    QCOMPARE(ruleWatch.timer()->interval(), (4 * 24 * 60 * 60 + 2 * 60 * 60) * 1000);
    QCOMPARE(*(ruleWatch.targetRuleItem()), item1);
}

void
TestRuleWatch::refreshWatch_dayTimeTests() {
    Rules rules;
    RuleWatch ruleWatch(&rules);
    QSet<int> days; // 0 - 6 (mon - su)
    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");

    // Check a rule with one day selected, with time 1 minute after now
    RuleItem item1;
    item1.ruleActive = true;
    days << 1; // Tuesday
    item1.rule.setDays(days);
    item1.rule.setTimeStart(now.time().addSecs(60));
    rules << item1;

    ruleWatch.refreshWatch(now);
    QVERIFY(ruleWatch.timer()->isActive() == true);
    QCOMPARE(ruleWatch.timer()->interval(), 60 * 1000);

    // Then try with a time that is a minute before. In that case wakeup should go to next week.
    rules.first().rule.setTimeStart(now.time().addSecs(-60));
    ruleWatch.refreshWatch(now);
    QVERIFY(ruleWatch.timer()->isActive() == true);
    QCOMPARE(ruleWatch.timer()->interval(), (7 * 24 * 60 * 60 - 60) * 1000);
}

void
TestRuleWatch::refreshWatch_multiRuleTests() {
    Rules rules;
    RuleWatch ruleWatch(&rules);
    // Tuesday
    QDateTime now = QDateTime::fromString("27.09.2011 10:00", "dd.MM.yyyy hh:mm");

    RuleItem item1;
    RuleItem item2;
    RuleItem item3;

    // Check that item1 is not matched, since rule is not active
    item1.ruleActive = false;
    item1.rule.setTimeStart(now.addSecs(60).time());
    // Check that if rules equal, the first one matching (in order) is used
    item2.ruleActive = true;
    item2.rule.setTimeStart(now.addSecs(120).time());
    item3.ruleActive = true;
    item3.rule.setTimeStart(item2.rule.getTimeStart());

    rules << item1 << item2 << item3;

    ruleWatch.refreshWatch(now);
    QVERIFY(ruleWatch.timer()->isActive() == true);
    QCOMPARE(ruleWatch.timer()->interval(), 120 * 1000);
    QCOMPARE(*(ruleWatch.targetRuleItem()), item2);
}

void
TestRuleWatch::activateRule_signalTest() {
    Rules rules;
    RuleWatch ruleWatch(&rules);
    // Tuesday
    QDateTime now = QDateTime::currentDateTime();

    RuleItem item;

    item.ruleActive = true;
    item.rule.setTimeStart(now.addSecs(1).time());

    rules << item;

    RuleWatchSignalTarget signalTarget;

    QCOMPARE(signalTarget.ruleActivated, (RuleItem *)NULL);
    QCOMPARE(signalTarget.numRuleActivated, 0);
    QVERIFY(ruleWatch.timer()->isActive() == false);

    connect(&ruleWatch, SIGNAL(activateRule(RuleItem)), &signalTarget, SLOT(onRuleActivated(RuleItem)));

    ruleWatch.refreshWatch(now);

    QCOMPARE(signalTarget.ruleActivated, (RuleItem *)NULL);
    QCOMPARE(signalTarget.numRuleActivated, 0);
    QVERIFY(ruleWatch.timer()->isActive() == true);
    QCOMPARE(ruleWatch.timer()->interval(), 1 * 1000);

    // Wait for 2 seconds, the sginal should be fired by then.
    do {
        QTest::qWait(2 * 1000);
    } while (QTime::currentTime() < now.addSecs(1).time());

    QCOMPARE(signalTarget.ruleActivated, &(rules[0]));
    QCOMPARE(signalTarget.numRuleActivated, 1);

//    ruleWatch.refreshWatch(now);
//    QVERIFY(ruleWatch.timer()->isActive() == true);
//    QCOMPARE(ruleWatch.timer()->interval(), 120 * 1000);
//    QCOMPARE(ruleWatch.targetRuleItem(), item2);
}
