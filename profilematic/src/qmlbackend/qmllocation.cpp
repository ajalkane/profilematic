#include "qmllocation.h"

#ifndef PROFILEMATIC_SIMULATED_CELLID
#include "location/qmllocationmobility.h"
#else
#include "location/qmllocationrandom.h"
#endif

QmlLocation::QmlLocation(QObject *parent)
    : QObject(parent)
{
}

QmlLocation *
QmlLocation::create() {
#ifndef PROFILEMATIC_SIMULATED_CELLID
    return new QmlLocationMobility;
#else
    return new QmlLocationRandom;
#endif
}

QmlLocation::~QmlLocation()
{
}
