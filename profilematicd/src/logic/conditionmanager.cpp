#include "conditionmanager.h"

ConditionManager::ConditionManager(QObject *parent)
    : QObject(parent)
{
}

ConditionManager::~ConditionManager()
{
}

void
ConditionManager::startRefresh()
{
}

void
ConditionManager::matchedRule(const Rule &rule)
{
}

void
ConditionManager::endRefresh()
{
}
