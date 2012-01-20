#include "conditionmanagerchain.h"

ConditionManagerChain::ConditionManagerChain(QObject *parent)
    : ConditionManager(parent)
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

bool
ConditionManagerChain::refresh(const Rule &rule) {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        if (!cm->refresh(rule)) {
            return false;
        }
    }
    return true;
}

void
ConditionManagerChain::matchedRule(const Rule &rule) {
    QList<ConditionManager *>::const_iterator i = _conditionManagers.constBegin();
    for (; i != _conditionManagers.constEnd(); ++i) {
        ConditionManager *cm = *i;
        cm->matchedRule(rule);
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
