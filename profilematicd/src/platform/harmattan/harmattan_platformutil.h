#ifndef HARMATTAN_PLATFORMUTIL_H
#define HARMATTAN_PLATFORMUTIL_H

#include "../platformutil.h"

class HarmattanPlatformUtil : public PlatformUtil
{
    Q_OBJECT

public:
    HarmattanPlatformUtil(QObject *parent = 0);
    virtual ~HarmattanPlatformUtil();

    virtual void setFlightMode(int flightMode);
};

#endif // PLATFORMUTIL_H
