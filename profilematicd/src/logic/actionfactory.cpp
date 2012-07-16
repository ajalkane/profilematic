#include "actionchain.h"
#include "actioncellularmode.h"
#include "actioncommandline.h"
#include "actionstandbyscreenmode.h"
#include "actionbackgroundconnections.h"
#include "actionflightmode.h"
#include "actionpowersavingmode.h"
#include "actionprofile.h"
#include "actionbluetooth.h"
#include "presence/actionpresence.h"

#include "actionfactory.h"

ActionFactory::ActionFactory()
{
}

Action *
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
    ac->add(new ActionCommandLine());
    ac->add(PlatformUtil::instance()->createActionPresence());
    return ac;
}
