#include "conditionmanagerchain.h"
#include "conditionmanagercharging.h"
#include "conditionmanagertime.h"
#include "conditionmanagerlocationcell.h"
#include "conditionmanagerwlan.h"
#include "conditionmanageridle.h"
#include "conditionmanagernfc.h"
#include "conditionmanagerinternetconnectionmode.h"
#include "conditionmanagerbatterylevel.h"
#include "conditionmanagertimeinterval.h"

#include "conditionmanagercaching.h"
#include "calendar/conditionmanagercalendar.h"

#include "conditionmanagerfactory.h"

ConditionManagerFactory::ConditionManagerFactory()
{
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
    cm << new ConditionManagerCaching(new ConditionManagerTimeInterval);

    return cm;
}
