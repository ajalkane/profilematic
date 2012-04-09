#include "conditionmanageridle.h"

ConditionManagerIdle::ConditionManagerIdle(QObject *parent)
    : ConditionManager(parent)
{
    connect(PlatformUtil::instance(), SIGNAL(userActivityIdleChanged(bool)), this, SLOT(userActivityIdleChanged(bool)));

    qDebug("ConditionManagerIdle::ConditionManagerIdle current isIdle %d", PlatformUtil::instance()->isUserActivityIdle());
}

ConditionManagerIdle::~ConditionManagerIdle() {
}

void
ConditionManagerIdle::userActivityIdleChanged(bool isIdle)
{
    qDebug("ConditionManagerIdle::userActivityIdleChanged %d", isIdle);
}

void
ConditionManagerIdle::startRefresh() {
}

bool
ConditionManagerIdle::refresh(const RuleCondition &rule) {
    return true;
}

void
ConditionManagerIdle::matchedRule(const RuleCondition &rule) {

}

void
ConditionManagerIdle::endRefresh() {

}
