#include "qmllocation.h"

// MEEGO_VERSION_MAJOR seems to be defined only for qmake, not for the compilation.
// So for now use __arm__ as ugly workaround.
// #ifdef MEEGO_VERSION_MAJOR
#ifdef __arm__
#include "platform/qmllocationmobility.h"
#else
#include "platform/qmllocationrandom.h"
#endif

QmlLocation::QmlLocation(QObject *parent)
    : QObject(parent)
{
}

QmlLocation *
QmlLocation::create() {
    // MEEGO_VERSION_MAJOR seems to be defined only for qmake, not for the compilation.
    // So for now use __arm__ as ugly workaround.
    // #ifdef MEEGO_VERSION_MAJOR
#ifdef __arm__
    return new QmlLocationMobility;
#else
    // TODO compile target specific classes
    return new QmlLocationRandom;
#endif
}

QmlLocation::~QmlLocation()
{
}
