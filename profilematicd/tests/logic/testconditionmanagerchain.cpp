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
#include "testconditionmanagerchain.h"
#include "signalcounter.h"

namespace {
    class DummyConditionManager : public ConditionManager {
    public:
        QList<Rule> matchingRules;        
        QList<Rule> refreshCalledWithRules;
        bool refreshCalled;
        bool emitRefreshNeeded;

        virtual void startRefresh() {
            if (emitRefreshNeeded) {
                emit refreshNeeded();
            }
        }

        virtual bool refresh(const Rule &rule) {
            refreshCalledWithRules << rule;
            refreshCalled = true;
            return matchingRules.contains(rule);
        }
    };    
}

const Rule *
refresh(ConditionManager &cm, const QList<Rule> &rules) {
    cm.startRefresh();
    QList<Rule>::const_iterator firstMatchingRule = rules.constBegin();
    for (; firstMatchingRule != rules.constEnd(); ++firstMatchingRule) {
        bool isMatching = cm.refresh(*firstMatchingRule);
        if (isMatching) {
            break;
        }
    }
    cm.endRefresh();
    return firstMatchingRule != rules.constEnd() ? &(*firstMatchingRule) : 0;
}

TestConditionManagerChain::TestConditionManagerChain() {
}

void
TestConditionManagerChain::refresh_withNoMatches() {
    DummyConditionManager *cm1 = new DummyConditionManager;
    DummyConditionManager *cm2 = new DummyConditionManager;
    DummyConditionManager *cm3 = new DummyConditionManager;
    DummyConditionManager *cm4 = new DummyConditionManager;
    Rule rule1;
    Rule rule2;
    Rule rule3;
    rule1.setRuleId("1");
    rule2.setRuleId("2");
    rule3.setRuleId("3");

    QList<Rule> expectCalled1;
    QList<Rule> expectCalled2;
    QList<Rule> expectCalled3;
    QList<Rule> expectCalled4;

    expectCalled1 << rule1 << rule2 << rule3;
    expectCalled2 << rule1 << rule2;
    expectCalled3 << rule1;

    cm1->matchingRules = expectCalled2;
    cm2->matchingRules = expectCalled3;
    cm3->matchingRules = expectCalled4;

    ConditionManagerChain cmc;
    cmc.add(cm1);
    cmc.add(cm2);
    cmc.add(cm3);
    cmc.add(cm4);

    const Rule *match = refresh(cmc, expectCalled1);

    QCOMPARE(cm1->refreshCalled, true);
    QCOMPARE(cm1->refreshCalledWithRules, expectCalled1);

    QCOMPARE(cm2->refreshCalled, true);
    QCOMPARE(cm2->refreshCalledWithRules, expectCalled2);

    QCOMPARE(cm3->refreshCalled, true);
    QCOMPARE(cm3->refreshCalledWithRules, expectCalled3);

    QCOMPARE(cm4->refreshCalled, false);
    QCOMPARE(cm4->refreshCalledWithRules, expectCalled4);

    QVERIFY(match == 0);
}

void
TestConditionManagerChain::refresh_withMatches() {
    DummyConditionManager *cm1 = new DummyConditionManager;
    DummyConditionManager *cm2 = new DummyConditionManager;
    Rule rule1;
    Rule rule2;
    Rule rule3;
    rule1.setRuleId("1");
    rule2.setRuleId("2");
    rule3.setRuleId("3");

    QList<Rule> expectCalled1;
    QList<Rule> expectCalled2;

    cm1->matchingRules << rule1 << rule2 << rule3;
    cm2->matchingRules << rule2;

    expectCalled1 << rule1 << rule2;
    expectCalled2 << rule1 << rule2;

    ConditionManagerChain cmc;
    cmc.add(cm1);
    cmc.add(cm2);

    const Rule *match = refresh(cmc, expectCalled1);

    QCOMPARE(cm1->refreshCalled, true);
    QCOMPARE(cm1->refreshCalledWithRules, expectCalled1);

    QCOMPARE(cm2->refreshCalled, true);
    QCOMPARE(cm2->refreshCalledWithRules, expectCalled2);

    QVERIFY(*match == rule2);
}

void
TestConditionManagerChain::refreshNeeded_signalTest() {
    DummyConditionManager *cm1 = new DummyConditionManager;
    DummyConditionManager *cm2 = new DummyConditionManager;
    SignalCounter signalTarget;
    ConditionManagerChain cmc;
    QList<Rule> rules;

    cm1->emitRefreshNeeded = true;
    cm2->emitRefreshNeeded = true;

    cmc.add(cm1);
    cmc.add(cm2);
    connect(&cmc, SIGNAL(refreshNeeded()), &signalTarget, SLOT(onSignal()));

    QCOMPARE(signalTarget.numSignal, 0);
    refresh(cmc, rules);
    QCOMPARE(signalTarget.numSignal, 2);
}
