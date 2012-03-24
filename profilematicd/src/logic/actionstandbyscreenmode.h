#ifndef ACTIONSTANDBYSCREENMODE_H
#define ACTIONSTANDBYSCREENMODE_H

#include "action.h"
#include "../platform/platformutil.h"

class GConfItem;

class ActionStandByScreenMode: public Action
{
    PlatformUtil *_platformUtil;
    int _previousSbsmState;

public:
    ActionStandByScreenMode(PlatformUtil *platformUtil);

    virtual void activate(const RuleAction &rule);
};

#endif // ACTIONSTANDBYSCREENMODE_H
