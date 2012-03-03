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
#include <RadioAccess>

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
        bool return_value = deviceMode.setMode(flightMode == 1
                                          ? MeeGo::QmDeviceMode::Flight
                                          : MeeGo::QmDeviceMode::Normal);

        qDebug("HarmattanPlatformUtil::Setting flightMode return_value %d", return_value);
    }
}

int
HarmattanPlatformUtil::powerSavingMode() const {
    MeeGo::QmDeviceMode deviceMode;
    MeeGo::QmDeviceMode::PSMState state = deviceMode.getPSMState();
    int psmState = -1;
    switch (state) {
    case MeeGo::QmDeviceMode::PSMStateOn:
        psmState = 1; break;
    case MeeGo::QmDeviceMode::PSMStateOff:
        psmState = 0; break;
    default:
        qDebug("HarmattanPlatformUtil::psmState warning unrecognized psmState %d", state);
    }

    qDebug("HarmattanPlatformUtil::psmState current Power Saving State %d", psmState);
    return psmState;
}

void
HarmattanPlatformUtil::setPowerSavingMode(int state) {
    MeeGo::QmDeviceMode deviceMode;

    qDebug("HarmattanPlatformUtil::Setting Power Saving State to %d", state);
    if (state == 0 || state == 1) {
        bool return_value = deviceMode.setPSMState(state == 1
                                          ? MeeGo::QmDeviceMode::PSMStateOn
                                          : MeeGo::QmDeviceMode::PSMStateOff);

        qDebug("HarmattanPlatformUtil::Setting Power Saving State, return value %d", return_value);
    }
}

int
HarmattanPlatformUtil::cellularMode() const {
    Cellular::RadioAccess radioAccess;
    Cellular::RadioAccess::Mode mode = radioAccess.mode();
    int cellularMode = -1;
    switch (mode) {
    case Cellular::RadioAccess::DualMode:
        cellularMode = 0; break;
    case Cellular::RadioAccess::OnlyTwoG:
        cellularMode = 1; break;
    case Cellular::RadioAccess::OnlyThreeG:
        cellularMode = 2; break;
    default:
        qDebug("HarmattanPlatformUtil::cellularMode warning unrecognized RadioAccess mode %d", cellularMode);
    }

    qDebug("HarmattanPlatformUtil::cellularMode current cellular mode %d", cellularMode);
    return cellularMode;
}

void
HarmattanPlatformUtil::setCellularMode(int cellularMode) {
    qDebug("HarmattanPlatformUtil::setCellularMode setting cellular mode %d", cellularMode);

    Cellular::RadioAccess radioAccess;
    Cellular::RadioAccess::Mode mode = Cellular::RadioAccess::Unknown;
    switch (cellularMode) {
    case 0:
        qDebug("HarmattanPlatformUtil::setCellularMode setting Dual Mode");
        mode = Cellular::RadioAccess::DualMode;
        break;
    case 1:
        qDebug("HarmattanPlatformUtil::setCellularMode setting 2G Mode");
        mode = Cellular::RadioAccess::OnlyTwoG;
        break;
    case 2:
        qDebug("HarmattanPlatformUtil::setCellularMode setting 3G Mode");
        mode = Cellular::RadioAccess::OnlyThreeG;
        break;
    default:
        qDebug("HarmattanPlatformUtil::setCellularMode warning unrecognized mode %d", cellularMode);
    }

    if (mode != Cellular::RadioAccess::Unknown) {
        radioAccess.setMode(mode);
    }
}

ActionPresence *HarmattanPlatformUtil::createActionPresence()
{
    return new ActionPresenceImpl();
}
