#include "conditionmanagerchain.h"

ConditionManagerChain::ConditionManagerChain()
{
}

ConditionManagerChain::~ConditionManagerChain()
{
    while (!_conditionManagers.isEmpty()) {
        ConditionManager *cm = _conditionManagers.back();
        _conditionManagers.pop_back();
        delete cm;
    }
}

void
ConditionManagerChain::startRefresh() {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        cm->startRefresh();
    }
}

void
ConditionManagerChain::refresh(const QList<Rule> &rules) {
    _matchingRules = rules;

    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        cm->refresh(_matchingRules);
        _matchingRules = cm->matchingRules();
    }
}

void
ConditionManagerChain::endRefresh() {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        cm->endRefresh();
    }
}

void
ConditionManagerChain::add(ConditionManager *conditionManager) {
    _conditionManagers.append(conditionManager);
    connect(conditionManager, SIGNAL(refreshNeeded()), this, SIGNAL(refreshNeeded()));
}
