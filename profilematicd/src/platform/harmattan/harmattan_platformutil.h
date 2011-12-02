#ifndef HARMATTAN_PLATFORMUTIL_H
#define HARMATTAN_PLATFORMUTIL_H

#include "../platformutil.h"

class HarmattanPlatformUtil : public PlatformUtil
{
public:
    HarmattanPlatformUtil();

    virtual void setFlightMode(int flightMode);
};

#endif // PLATFORMUTIL_H
