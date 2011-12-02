#ifndef PLATFORMUTIL_H
#define PLATFORMUTIL_H

class PlatformUtil
{
protected:
    PlatformUtil();
public:
    static PlatformUtil *create();
    virtual ~PlatformUtil();

    virtual void setFlightMode(int flightMode);
};

#endif // PLATFORMUTIL_H
