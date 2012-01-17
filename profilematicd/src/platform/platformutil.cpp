#include <QtGlobal>

#include "platformutil.h"

// MEEGO_VERSION_MAJOR seems to be defined only for qmake, not for the compilation.
// So for now use __arm__ as ugly workaround.
// #ifdef MEEGO_VERSION_MAJOR
#ifdef __arm__
#include "harmattan/harmattan_platformutil.h"
#endif

PlatformUtil::PlatformUtil(QObject *parent)
    : QObject(parent)
{
}

PlatformUtil::~PlatformUtil()
{
}

PlatformUtil *
PlatformUtil::create()
{
    // MEEGO_VERSION_MAJOR seems to be defined only for qmake, not for the compilation.
    // So for now use __arm__ as ugly workaround.
    // #ifdef MEEGO_VERSION_MAJOR
#ifdef __arm__
    return new HarmattanPlatformUtil;
#endif
    // TODO compile target specific classes
    return new PlatformUtil;
}

void
PlatformUtil::setFlightMode(int flightMode) {
    qDebug("PlatformUtil::setFlightMode default, doing nothing for value %d", flightMode);
}
