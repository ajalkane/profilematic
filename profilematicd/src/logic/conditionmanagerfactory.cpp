#include "conditionmanagerchain.h"
#include "conditionmanagercharging.h"
#include "conditionmanagertime.h"
#include "conditionmanagerlocationcell.h"
#include "conditionmanagerwlan.h"
#include "conditionmanageridle.h"
#include "conditionmanagernfc.h"
#include "conditionmanagerinternetconnectionmode.h"
#include "conditionmanagerbatterylevel.h"

#include "conditionmanagercaching.h"
#include "calendar/conditionmanagercalendar.h"

#include "conditionmanagerfactory.h"

ConditionManagerFactory::ConditionManagerFactory()
{
}

ConditionManager *
ConditionManagerFactory::create()
{
    ConditionManagerChain *cm = new ConditionManagerChain();
    cm->add(new ConditionManagerCaching(new ConditionManagerCharging()));
    cm->add(new ConditionManagerCaching(new ConditionManagerTime));
    cm->add(new ConditionManagerCaching(new ConditionManagerLocationCell()));
    cm->add(new ConditionManagerCaching(new ConditionManagerWlan()));
    cm->add(new ConditionManagerCaching(new ConditionManagerIdle()));
    cm->add(new ConditionManagerCaching(new ConditionManagerNFC()));
    cm->add(new ConditionManagerInternetConnectionMode());
    cm->add(new ConditionManagerCaching(new ConditionManagerBatteryLevel()));
    cm->add(new ConditionManagerCaching(new ConditionManagerCalendar));

    return cm;
}
