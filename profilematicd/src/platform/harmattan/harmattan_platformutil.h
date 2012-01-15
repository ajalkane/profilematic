#ifndef HARMATTAN_PLATFORMUTIL_H
#define HARMATTAN_PLATFORMUTIL_H

#include <QSystemNetworkInfo>

QTM_USE_NAMESPACE

#include "../platformutil.h"

class HarmattanPlatformUtil : public PlatformUtil
{
    Q_OBJECT

    QSystemNetworkInfo _networkInfo;

public:
    HarmattanPlatformUtil(QObject *parent = 0);
    virtual ~HarmattanPlatformUtil();

    virtual void setFlightMode(int flightMode);

    virtual int cellId();

    virtual void monitorCellId(bool monitor);
};

#endif // PLATFORMUTIL_H
