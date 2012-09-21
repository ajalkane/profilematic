#include "conditionmanagercharging.h"

ConditionManagerCharging::ConditionManagerCharging(QObject *parent)
    : ConditionManager(parent), _hasChargingConditions(false)
{
    qDebug("ConditionManagerCharging::constructor");
    _currentChargingState = (RuleCondition::ChargingState)PlatformUtil::instance()->batteryChargingState();
    connect(PlatformUtil::instance(), SIGNAL(batteryChargingStateChanged(int)), this, SLOT(batteryChargingStateChanged(int)));
}

void
ConditionManagerCharging::startRefresh() {
    _hasChargingConditions = false;
}

bool
ConditionManagerCharging::refresh(const Rule::IdType &, const RuleCondition &condition) {
    RuleCondition::ChargingState chargingState = condition.getChargingState();
    if (chargingState == RuleCondition::UndefinedChargingState) {
        qDebug("ConditionManagerCharging charging state not set");
        return true;
    }

    _hasChargingConditions = true;

    qDebug("ConditionManagerCharging::refresh current %d rule %d", _currentChargingState, chargingState);
    return _currentChargingState == chargingState;
}

void
ConditionManagerCharging::endRefresh() {
}

void
ConditionManagerCharging::batteryChargingStateChanged(int chargingState) {
    qDebug("%s ConditionManagerCharging::batteryChargingStateChanged(%d) hasChargingConditions %d",
           qPrintable(QDateTime::currentDateTime().toString()), chargingState, _hasChargingConditions);

    _currentChargingState = (RuleCondition::ChargingState)chargingState;
    if (_hasChargingConditions) {
        emit refreshNeeded();
    }
}

