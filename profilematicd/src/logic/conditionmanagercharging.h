#ifndef CONDITIONMANAGERCHARGING_H
#define CONDITIONMANAGERCHARGING_H

#include "conditionmanager.h"
#include "../platform/platformutil.h"

class ConditionManagerCharging : public ConditionManager
{
    Q_OBJECT

    bool _hasChargingConditions;
    RuleCondition::ChargingState _currentChargingState;

public:
    ConditionManagerCharging(QObject *parent = 0);

    virtual void startRefresh();
    virtual bool refresh(const RuleCondition &rule);
    virtual void endRefresh();

private slots:
    void batteryChargingStateChanged(int chargingState);

};

#endif // CONDITIONMANAGERCHARGING_H
