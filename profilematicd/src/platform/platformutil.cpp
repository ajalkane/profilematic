/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
 *
 * This file is part of ProfileMatic.
 *
 * ProfileMatic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProfileMatic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProfileMatic.  If not, see <http://www.gnu.org/licenses/>
**/
#include <cstdio>
#include <cstdlib>

#include <QtGlobal>

#include "platformutil.h"

// MEEGO_VERSION_MAJOR seems to be defined only for qmake, not for the compilation.
// So for now use __arm__ as ugly workaround.
// #ifdef MEEGO_VERSION_MAJOR
#ifdef __arm__
#include "harmattan/harmattan_platformutil.h"
#else
#include "../logic/presence/actionpresencestub.h"
#endif

PlatformUtil *PlatformUtil::_instance = 0;

PlatformUtil::PlatformUtil(QObject *parent)
    : QObject(parent)
{
}

PlatformUtil::~PlatformUtil()
{
}

void
PlatformUtil::initialize() {
    if (_instance != 0) {
        fprintf(stderr, "PlatformUtil::initialize called more than once");
        exit(1);
    }

    _instance = PlatformUtil::_create();
}

void
PlatformUtil::deinitialize() {
    delete _instance;
    _instance = 0;
}

PlatformUtil *
PlatformUtil::_create()
{
    // MEEGO_VERSION_MAJOR seems to be defined only for qmake, not for the compilation.
    // So for now use __arm__ as ugly workaround.
    // #ifdef MEEGO_VERSION_MAJOR
#ifdef __arm__
    return new HarmattanPlatformUtil;
#endif
    return new PlatformUtil;
}

int
PlatformUtil::flightMode() const {
    qDebug("PlatformUtil::flightMode default, returning -1");
    return -1;
}

void
PlatformUtil::setFlightMode(int flightMode) {
    qDebug("PlatformUtil::setFlightMode default, doing nothing for value %d", flightMode);
}

int
PlatformUtil::powerSavingMode() const {
    qDebug("PlatformUtil::powerSavingMode default, returning -1");
    return -1;
}

void
PlatformUtil::setPowerSavingMode(int state) {
    qDebug("PlatformUtil::setPowerSavingMode default, doing nothing for value %d", state);
}

bool
PlatformUtil::hasDeviceModeCredential() const {
    qDebug("PlatformUtil::hasDeviceModeCredential() default");
    return false;
}

int
PlatformUtil::cellularMode() const {
    qDebug("PlatformUtil::cellularMode default, returning -1");
    return -1;
}

void
PlatformUtil::setCellularMode(int state) {
    qDebug("PlatformUtil::setCellularMode default, doing nothing for value %d", state);
}

int
PlatformUtil::cellularActivity() const {
    qDebug("PlatformUtil::cellularActivity default, returning -1");
    return -1;
}

void
PlatformUtil::setStandByScreenMode(int mode) {
    qDebug("PlatformUtil::setStandByScreenMode default, doing nothing for value %d", mode);
}

int
PlatformUtil::standByScreenMode() const {
    qDebug("PlatformUtil::standByScreenMode default, returning -1");
    return -1;
}

void
PlatformUtil::setBackgroundConnectionsMode(int mode) {
    qDebug("PlatformUtil::setBackgroundConnectionsMode default, doing nothing for value %d", mode);
}

int
PlatformUtil::backgroundConnectionsMode() const {
    qDebug("PlatformUtil::backgroundConnectionsMode default, returning -1");
    return -1;
}

int
PlatformUtil::batteryChargingState() const {
    qDebug("PlatformUtil::batteryChargingState default, returning -1");
    return -1;
}

void
PlatformUtil::publishNotification(const QString &message) {
    qDebug("PlatformUtil::publishNotification default, doing nothing for value %s", qPrintable(message));
}

bool
PlatformUtil::isUserActivityIdle() {
    qDebug("PlatformUtil::isUserActivityIdle default, returning false");
    return false;
}

ActionPresence *PlatformUtil::createActionPresence()
{
#ifndef __arm__
    return new ActionPresenceStub();
#else
    return NULL;
#endif
}

int
PlatformUtil::batteryLevel() const {
    qDebug("PlatformUtil::batteryLevel default, returning -1");
    return -1;
}

void
PlatformUtil::monitorBatteryLevel(bool monitor) {
    qDebug("PlatformUtil::monitorBatteryLevel default, monitor: %d", monitor);
}


