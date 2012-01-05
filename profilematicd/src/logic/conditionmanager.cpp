#include "conditionmanager.h"

ConditionManager::ConditionManager()
{
}

ConditionManager::~ConditionManager()
{
    _matchingRules.clear();
}

void
ConditionManager::startRefresh()
{
    _matchingRules.clear();
}

void
ConditionManager::endRefresh()
{
    _matchingRules.clear();
}
