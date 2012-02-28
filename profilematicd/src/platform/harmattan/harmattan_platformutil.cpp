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
#include "harmattan_platformutil.h"
#include "../../logic/presence/actionpresenceimpl.h"
#include <qmdevicemode.h>

HarmattanPlatformUtil::HarmattanPlatformUtil(QObject *parent)
    : PlatformUtil(parent)
{
}

HarmattanPlatformUtil::~HarmattanPlatformUtil() {}

int
HarmattanPlatformUtil::flightMode() const {
    MeeGo::QmDeviceMode deviceMode;
    MeeGo::QmDeviceMode::DeviceMode mode = deviceMode.getMode();
    int flightMode = -1;
    switch (mode) {
    case MeeGo::QmDeviceMode::Flight:
        flightMode = 1; break;
    case MeeGo::QmDeviceMode::Normal:
        flightMode = 0; break;
    default:
        qDebug("HarmattanPlatformUtil::flightMode warning unrecognized deviceMode %d", mode);
    }

    qDebug("HarmattanPlatformUtil::flightMode current deviceMode %d", flightMode);
    return flightMode;
}

void
HarmattanPlatformUtil::setFlightMode(int flightMode) {
    MeeGo::QmDeviceMode deviceMode;

    qDebug("HarmattanPlatformUtil::Setting flightMode to %d", flightMode);
    if (flightMode == 0 || flightMode == 1) {
        bool success = deviceMode.setMode(flightMode == 1
                                          ? MeeGo::QmDeviceMode::Flight
                                          : MeeGo::QmDeviceMode::Normal);

        qDebug("HarmattanPlatformUtil::Setting flightMode success %d", success);
    }
}

ActionPresence *HarmattanPlatformUtil::createActionPresence()
{
    return new ActionPresenceImpl();
}
