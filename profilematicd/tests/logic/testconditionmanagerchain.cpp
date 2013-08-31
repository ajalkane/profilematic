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
        QList<Rule::IdType> matchingRules;
        QList<Rule::IdType> refreshCalledWithRules;
        bool refreshCalled;
        bool emitRefreshNeeded;

        virtual bool conditionSetForMatching(const RuleCondition &cond) const {
            return true;
        }

        virtual void startRefresh() {
            if (emitRefreshNeeded) {
                emit refreshNeeded();
            }
        }

        virtual bool refresh(const Rule::IdType &ruleId, const RuleCondition &rule) {
            refreshCalledWithRules << ruleId;
            refreshCalled = true;
            return matchingRules.contains(ruleId);
        }
    };    
}

const Rule::IdType *
refresh(ConditionManager &cm, const QList<Rule::IdType> &ruleIds) {
    cm.startRefresh();
    QList<Rule::IdType>::const_iterator firstMatchingRuleId = ruleIds.constBegin();
    for (; firstMatchingRuleId != ruleIds.constEnd(); ++firstMatchingRuleId) {
        bool isMatching = cm.refresh(*firstMatchingRuleId, RuleCondition());
        if (isMatching) {
            break;
        }
    }
    cm.endRefresh();
    return firstMatchingRuleId != ruleIds.constEnd() ? &(*firstMatchingRuleId) : 0;
}

TestConditionManagerChain::TestConditionManagerChain() {
}

void
TestConditionManagerChain::refresh_withNoMatches() {
    DummyConditionManager *cm1 = new DummyConditionManager;
    DummyConditionManager *cm2 = new DummyConditionManager;
    DummyConditionManager *cm3 = new DummyConditionManager;
    DummyConditionManager *cm4 = new DummyConditionManager;
    Rule::IdType rule1("1");
    Rule::IdType rule2("2");
    Rule::IdType rule3("3");

    QList<Rule::IdType> expectCalled1;
    QList<Rule::IdType> expectCalled2;
    QList<Rule::IdType> expectCalled3;
    QList<Rule::IdType> expectCalled4;

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

    const Rule::IdType *match = refresh(cmc, expectCalled1);

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
    Rule::IdType rule1("1");
    Rule::IdType rule2("2");
    Rule::IdType rule3("3");

    QList<Rule::IdType> expectCalled1;
    QList<Rule::IdType> expectCalled2;

    cm1->matchingRules << rule1 << rule2 << rule3;
    cm2->matchingRules << rule2;

    expectCalled1 << rule1 << rule2;
    expectCalled2 << rule1 << rule2;

    ConditionManagerChain cmc;
    cmc.add(cm1);
    cmc.add(cm2);

    const Rule::IdType *match = refresh(cmc, expectCalled1);

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
    QList<Rule::IdType> ruleIds;

    cm1->emitRefreshNeeded = true;
    cm2->emitRefreshNeeded = true;

    cmc.add(cm1);
    cmc.add(cm2);
    connect(&cmc, SIGNAL(refreshNeeded()), &signalTarget, SLOT(onSignal()));

    QCOMPARE(signalTarget.numSignal, 0);
    refresh(cmc, ruleIds);
    QCOMPARE(signalTarget.numSignal, 2);
}
