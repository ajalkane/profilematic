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

ConditionManagerChain *
ConditionManagerFactory::create()
{
    ConditionManagerChain *cm = new ConditionManagerChain();
    cm->add(new ConditionManagerCaching(new ConditionManagerCharging()));
    cm->add(new ConditionManagerCaching(new ConditionManagerTime));
    cm->add(new ConditionManagerCaching(new ConditionManagerLocationCell()));
    cm->add(new ConditionManagerCaching(new ConditionManagerWlan()));
    cm->add(new ConditionManagerCaching(new ConditionManagerIdle()));
    cm->add(new ConditionManagerCaching(new ConditionManagerNFC()));
    cm->add(new ConditionManagerCaching(new ConditionManagerInternetConnectionMode()));
    cm->add(new ConditionManagerCaching(new ConditionManagerBatteryLevel()));
    cm->add(new ConditionManagerCaching(new ConditionManagerCalendar));

    return cm;
}

QList<ConditionManager *>
ConditionManagerFactory::createAsList()
{
    QList<ConditionManager *> cm;
    cm << new ConditionManagerCaching(new ConditionManagerCharging());
    cm << new ConditionManagerCaching(new ConditionManagerTime);
    cm << new ConditionManagerCaching(new ConditionManagerLocationCell());
    cm << new ConditionManagerCaching(new ConditionManagerWlan());
    cm << new ConditionManagerCaching(new ConditionManagerIdle());
    cm << new ConditionManagerCaching(new ConditionManagerNFC());
    cm << new ConditionManagerCaching(new ConditionManagerInternetConnectionMode());
    cm << new ConditionManagerCaching(new ConditionManagerBatteryLevel());
    cm << new ConditionManagerCaching(new ConditionManagerCalendar);

    return cm;
}
