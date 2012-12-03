#include "testconditionmanagercaching.h"

class MockCacheable : public ConditionManagerCacheable {
public:
    int numStartMonitor;
    int numStopMonitor;
    int numRulesChanged;
    int numMatch;

    MatchStatus returnMatch;
    bool returnConditionSetForMatching;

    MockCacheable() {
        setObjectName("MockCacheable");
        reset();
    }

    void reset() {
        numStartMonitor = numStopMonitor = numRulesChanged = numMatch = 0;
        returnMatch = Matched;
        returnConditionSetForMatching = true;
    }

    bool conditionSetForMatching(const RuleCondition &cond) const {
        Q_UNUSED(cond)
        return returnConditionSetForMatching;
    }

    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond) {
        Q_UNUSED(ruleId)
        Q_UNUSED(cond)
        ++numMatch;
        return returnMatch;
    }

    virtual void startMonitor() {
        ++numStartMonitor;
    }
    virtual void stopMonitor() {
        ++numStopMonitor;
    }

    virtual void rulesChanged()  {
        ++numRulesChanged;
    }

    void emitMatchInvalidated() {
        emit matchInvalidated();
    }
};

TestConditionManagerCaching::TestConditionManagerCaching()
{
}

void
TestConditionManagerCaching::testStartStopMonitor() {
    MockCacheable *mock = new MockCacheable();
    ConditionManagerCaching cmc(mock);
    Rule::IdType ruleId1("1");
    Rule::IdType ruleId2("2");
    RuleCondition cond;

    QCOMPARE(mock->numStartMonitor, 0);
    QCOMPARE(mock->numStopMonitor, 0);

    // Tests that if match returns only notSet, monitoring is not turned on
    mock->returnMatch = ConditionManagerCacheable::MatchNotSet;
    cmc.startRefresh();;
    cmc.refresh(ruleId1, cond);
    cmc.endRefresh();
    QCOMPARE(mock->numStartMonitor, 0);
    QCOMPARE(mock->numStopMonitor, 0);

    // Tests that if at least one returns Matched, monitoring is turned on
    // Invalidate caches
    mock->emitMatchInvalidated();
    cmc.startRefresh();
    mock->returnMatch = ConditionManagerCacheable::Matched;
    cmc.refresh(ruleId1, cond);
    mock->returnMatch = ConditionManagerCacheable::MatchNotSet;
    cmc.refresh(ruleId2, cond);
    cmc.endRefresh();
    QCOMPARE(mock->numStartMonitor, 1);
    QCOMPARE(mock->numStopMonitor, 0);

    // Then test again that monitoring is not turned on if it already is on
    // Invalidate caches
    mock->emitMatchInvalidated();
    mock->returnMatch = ConditionManagerCacheable::Matched;
    cmc.startRefresh();;
    cmc.refresh(ruleId1, cond);
    cmc.endRefresh();
    QCOMPARE(mock->numStartMonitor, 1);
    QCOMPARE(mock->numStopMonitor, 0);

    // Then test again that monitoring is not turned on if NotMatched returned
    // Invalidate caches
    mock->emitMatchInvalidated();
    mock->returnMatch = ConditionManagerCacheable::NotMatched;
    cmc.startRefresh();
    cmc.refresh(ruleId1, cond);
    cmc.endRefresh();
    QCOMPARE(mock->numStartMonitor, 1);
    QCOMPARE(mock->numStopMonitor, 0);

    // Test that monitoring is turned off if only MatchNotSet returned
    // Invalidate caches
    mock->emitMatchInvalidated();
    mock->returnMatch = ConditionManagerCacheable::MatchNotSet;
    cmc.startRefresh();
    cmc.refresh(ruleId1, cond);
    cmc.refresh(ruleId2, cond);
    cmc.endRefresh();
    QCOMPARE(mock->numStartMonitor, 1);
    QCOMPARE(mock->numStopMonitor, 1);

    // Tests that if at least one returns NotMatched, monitoring is turned back on
    // Invalidate caches
    mock->emitMatchInvalidated();
    cmc.startRefresh();
    mock->returnMatch = ConditionManagerCacheable::NotMatched;
    cmc.refresh(ruleId1, cond);
    mock->returnMatch = ConditionManagerCacheable::MatchNotSet;
    cmc.refresh(ruleId2, cond);
    cmc.endRefresh();
    QCOMPARE(mock->numStartMonitor, 2);
    QCOMPARE(mock->numStopMonitor, 1);
}

void
TestConditionManagerCaching::testMatchCaching() {
    MockCacheable *mock = new MockCacheable();
    ConditionManagerCaching cmc(mock);
    Rule::IdType ruleId1("1");
    Rule::IdType ruleId2("2");
    Rule::IdType ruleId3("3");
    RuleCondition cond;

    // Test caching. First call goes through
    cmc.startRefresh();
    {
        mock->returnMatch = ConditionManagerCacheable::MatchNotSet;
        QCOMPARE(cmc.refresh(ruleId1, cond), true);
        QCOMPARE(mock->numMatch, 1);

        mock->returnMatch = ConditionManagerCacheable::Matched;
        QCOMPARE(cmc.refresh(ruleId2, cond), true);
        QCOMPARE(mock->numMatch, 2);

        mock->returnMatch = ConditionManagerCacheable::NotMatched;
        QCOMPARE(cmc.refresh(ruleId3, cond), false);
        QCOMPARE(mock->numMatch, 3);
    }
    cmc.endRefresh();

    // Test caching. Subsequent calls are returned from cache.
    cmc.startRefresh();
    {
        mock->returnMatch = ConditionManagerCacheable::MatchNotSet;
        QCOMPARE(cmc.refresh(ruleId1, cond), true);
        QCOMPARE(mock->numMatch, 3);

        mock->returnMatch = ConditionManagerCacheable::Matched;
        QCOMPARE(cmc.refresh(ruleId2, cond), true);
        QCOMPARE(mock->numMatch, 3);

        mock->returnMatch = ConditionManagerCacheable::NotMatched;
        QCOMPARE(cmc.refresh(ruleId3, cond), false);
        QCOMPARE(mock->numMatch, 3);
    }
    cmc.endRefresh();

    // Test caching. If cacheable tells match is invalidated, then cache is cleared
    mock->emitMatchInvalidated();
    cmc.startRefresh();
    {
        mock->returnMatch = ConditionManagerCacheable::MatchNotSet;
        QCOMPARE(cmc.refresh(ruleId1, cond), true);
        QCOMPARE(mock->numMatch, 4);

        mock->returnMatch = ConditionManagerCacheable::Matched;
        QCOMPARE(cmc.refresh(ruleId2, cond), true);
        QCOMPARE(mock->numMatch, 5);

        mock->returnMatch = ConditionManagerCacheable::NotMatched;
        QCOMPARE(cmc.refresh(ruleId3, cond), false);
        QCOMPARE(mock->numMatch, 6);
    }
    cmc.endRefresh();

    // Test caching. If ruleUpdated invalidate cache for it
    Rule updatedRule;
    updatedRule.setRuleId(ruleId2);
    cmc.ruleUpdated(updatedRule, updatedRule);
    cmc.startRefresh();
    {
        mock->returnMatch = ConditionManagerCacheable::MatchNotSet;
        QCOMPARE(cmc.refresh(ruleId1, cond), true);
        QCOMPARE(mock->numMatch, 6);

        mock->returnMatch = ConditionManagerCacheable::Matched;
        QCOMPARE(cmc.refresh(ruleId2, cond), true);
        QCOMPARE(mock->numMatch, 7);

        mock->returnMatch = ConditionManagerCacheable::NotMatched;
        QCOMPARE(cmc.refresh(ruleId3, cond), false);
        QCOMPARE(mock->numMatch, 7);
    }
    cmc.endRefresh();

}
