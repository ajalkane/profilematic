#include "conditionmanagerchain.h"
#include "conditionmanagercharging.h"
#include "conditionmanagertime.h"
#include "conditionmanagerlocationcell.h"
#include "conditionmanagerwlan.h"
#include "conditionmanageridle.h"
#include "conditionmanagernfc.h"
#include "conditionmanagerinternetconnectionmode.h"
#include "conditionmanagerbatterylevel.h"

#include "conditionmanagerfactory.h"

ConditionManagerFactory::ConditionManagerFactory()
{
}

ConditionManager *
ConditionManagerFactory::create()
{
    ConditionManagerChain *cm = new ConditionManagerChain();
    cm->add(new ConditionManagerCharging());
    cm->add(new ConditionManagerTime());
    cm->add(new ConditionManagerLocationCell());
    cm->add(new ConditionManagerWlan());
    cm->add(new ConditionManagerIdle());
    cm->add(new ConditionManagerNFC());
    cm->add(new ConditionManagerInternetConnectionMode());
    cm->add(new ConditionManagerBatteryLevel());

    return cm;
}
