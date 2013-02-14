#include "actionchain.h"
#include "actioncellularmode.h"
#include "actioncommandline.h"
#include "actionstandbyscreenmode.h"
#include "actionbackgroundconnections.h"
#include "actionflightmode.h"
#include "actionpowersavingmode.h"
#include "actionprofile.h"
#include "actionbluetooth.h"
#include "actiondevicevolume.h"
#include "actionalarm.h"
#include "application/actionapplication.h"
#include "presence/actionpresence.h"

#include "actionfactory.h"

ActionFactory::ActionFactory()
{
}

ActionChain *
ActionFactory::create(ProfileClient *profileClient) {
    ActionChain *ac = new ActionChain();
    ac->add(new ActionProfile(profileClient));
    ac->add(new ActionFlightMode());
    ac->add(new ActionPowerSavingMode());
    ac->add(new ActionBlueTooth());
    ac->add(new ActionCellularMode());
    ac->add(new ActionStandByScreenMode());
    // Modifying background connections do not work, disable until finding working solutions
    // ac->add(new ActionBackgroundConnections());
    ac->add(new ActionApplication());
    ac->add(new ActionCommandLine());
    ac->add(new ActionDeviceVolume());
    ac->add(new ActionAlarm());
    ac->add(PlatformUtil::instance()->createActionPresence());
    return ac;
}

QList<Action *>
ActionFactory::createAsList(ProfileClient *profileClient) {
    QList<Action *> ac;
    ac << new ActionProfile(profileClient);
    ac << new ActionFlightMode();
    ac << new ActionPowerSavingMode();
    ac << new ActionBlueTooth();
    ac << new ActionCellularMode();
    ac << new ActionStandByScreenMode();
    // Modifying background connections do not work, disable until finding working solutions
    // ac->add(new ActionBackgroundConnections());
    ac << new ActionApplication();
    ac << new ActionCommandLine();
    ac << new ActionDeviceVolume();
    ac << new ActionAlarm();
    ac << PlatformUtil::instance()->createActionPresence();

    return ac;
}

