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
// #include <MNotification>

#include "harmattan_platformutil.h"
#include "../../logic/presence/actionpresenceimpl.h"
#include <qmdevicemode.h>
#include <RadioAccess>
#include <QProcess>
#include <gconfitem.h>

#define GCONF_STAND_BY_SCREEN_KEY "/system/osso/dsm/display/use_low_power_mode"
#define GCONF_BACKGROUND_CONNECTIONS_DISABLED_KEY "/system/osso/connectivity/network_type/restricted_mode"
#define PLATFORMUTIL_APP "/opt/profilematic/bin/platformutil"
#define FLIGHT_MODE_CMD "invoker --type=m " PLATFORMUTIL_APP

HarmattanPlatformUtil::HarmattanPlatformUtil(QObject *parent)
    : PlatformUtil(parent), _pendingCellularMode(-1)
{
    _currentCellularActivity = _cellularControl.activity();
    _currentActivity = _qmActivity.get();
    _currentIdle = _currentActivity == MeeGo::QmActivity::Inactive;
    connect(&_cellularControl, SIGNAL(activityChanged(int)), this, SLOT(privateCellularActivityChanged(int)));
    connect(&_qmActivity, SIGNAL(activityChanged(MeeGo::QmActivity::Activity)), this, SLOT(activityChanged(MeeGo::QmActivity::Activity)));
    connect(&_systemState, SIGNAL(systemStateChanged(MeeGo::QmSystemState::StateIndication)), this, SLOT(privateSystemStateChanged(MeeGo::QmSystemState::StateIndication)));
    connect(&_qmbattery, SIGNAL(chargingStateChanged(MeeGo::QmBattery::ChargingState)), this, SLOT(privateBatteryChargingStateChanged(MeeGo::QmBattery::ChargingState)));
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
    qDebug("HarmattanPlatformUtil::Setting flightMode to %d", flightMode);
    const char *par = flightMode == 1 ? " flightModeOn" : " flightModeOff";

    QString cmd(FLIGHT_MODE_CMD);
    cmd += par;

    int ret = QProcess::execute(cmd);
    qDebug("HarmattanPlatformUtil::setFlightMode() exec return code %d", ret);
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

bool
HarmattanPlatformUtil::hasDeviceModeCredential() const {
    // Detecting this is a bit harder now as using invoker. Assuming the best.
    return true;
    //    int exitCode = execPlatformApp("hasDeviceModeCredential");
    //    if (exitCode < 0) {
    //        qWarning("Could not query mce::DeviceModeControl");
    //        return true;
    //    }
    //    return exitCode == 0;
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
    if (_currentCellularActivity > 0) {
        // Delay changing of cellular mode until no calls or data transfer. Otherwise
        // call/transfer aborts.
        qDebug("HarmattanPlatformUtil::setCellularMode cellularActivity is %d, pending the change of mode", _currentCellularActivity);
        _pendingCellularMode = cellularMode;
        return;
    }
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

int
HarmattanPlatformUtil::cellularActivity() const {
    int activity = (int)_cellularControl.activity();
    qDebug("HarmattanPlatformUtil::cellularActivity %d", activity);
    return activity;
}

void
HarmattanPlatformUtil::setStandByScreenMode(int mode) {
    qDebug("HarmattanPlatformUtil::setStandByScreenMode %d", mode);
    GConfItem gconfItem(GCONF_STAND_BY_SCREEN_KEY);
    if (mode == 0 || mode == 1) {
        gconfItem.set(mode);
    } else {
        qDebug("HarmattanPlatformUtil::setStandByScreenMode unsupported mode %d", mode);
    }
}

int
HarmattanPlatformUtil::standByScreenMode() const {
    GConfItem gconfItem(GCONF_STAND_BY_SCREEN_KEY);
    return gconfItem.value().toInt();
}


void
HarmattanPlatformUtil::setBackgroundConnectionsMode(int mode) {
    qDebug("HarmattanPlatformUtil::setBackgroundConnectionsMode %d", mode);
    GConfItem gconfItem(GCONF_BACKGROUND_CONNECTIONS_DISABLED_KEY);
    if (mode == 0 || mode == 1) {
        gconfItem.set((mode + 1) % 2);
    } else {
        qDebug("HarmattanPlatformUtil::setBackgroundConnectionsMode unsupported mode %d", mode);
    }
}

int
HarmattanPlatformUtil::backgroundConnectionsMode() const {
    GConfItem gconfItem(GCONF_BACKGROUND_CONNECTIONS_DISABLED_KEY);
    int disabled = gconfItem.value().toInt();
    switch (disabled) {
    case 0:
        // Enabled
        return 1;
    case 1:
        // Disabled
        return 0;
    }
    qWarning("HarmattanPlatformUtil::backgroundConnectionsMode() unrecognized value %d, returning -1", disabled);
    return -1;
}

int
HarmattanPlatformUtil::batteryChargingState() const {
    qDebug("HarmattanPlatformUtil::batteryChargingState");
    return _qmbattery.getChargingState();
}

void
HarmattanPlatformUtil::publishNotification(const QString &) {
// Forget about this for now... requires libraries that I'm not sure I want to link into for daemon.
// At least as long as it's only used for "Custom action" which is intended for "advanced usage".
//    qDebug("HarmattanPlatformUtil::publishNotification publishing notification '%s'", qPrintable(message));
//    MNotification notification(MNotification::DeviceEvent);
//    notification.setImage("/usr/share/icons/hicolor/80x80/apps/profilematic80.png");
//    notification.setBody("message");
//    notification.publish();
}

bool
HarmattanPlatformUtil::isUserActivityIdle() {
    return _currentIdle;
}

void
HarmattanPlatformUtil::_emitRealIdle() {
    qDebug("HarmattanPlatformUtil::_emitRealIdle: a(%d) c(%d) i(%d)", _currentActivity, _currentCellularActivity, _currentIdle);

    // Harmattan PR1.2 claims idle mode if on the phone! That's not nice, so this is a work around for that.
    // Basically not signaling idle if call is in progress.
    bool callInProgress = _currentCellularActivity == Cellular::SystemControl::Call
                       || _currentCellularActivity == Cellular::SystemControl::Signaling;

    if (_currentIdle == false && (!callInProgress && _currentActivity == MeeGo::QmActivity::Inactive)) {
        _currentIdle = true;
        emit userActivityIdleChanged(true);
    } else if (_currentIdle == true && (callInProgress || _currentActivity == MeeGo::QmActivity::Active)) {
        _currentIdle = false;
        emit userActivityIdleChanged(false);
    }
}

void
HarmattanPlatformUtil::activityChanged(MeeGo::QmActivity::Activity activity)
{
    _currentActivity = activity;

    // Harmattan PR1.2 claims idle mode if on the phone! That's not nice, so this is a work around for that
    _emitRealIdle();
}

void
HarmattanPlatformUtil::privateCellularActivityChanged(int activity)
{
    _currentCellularActivity = (Cellular::SystemControl::Activity)activity;
    qDebug("HarmattanPlatformUtil::cellularActivityChanged %d", activity);
    emit cellularActivityChanged(activity);

    // Harmattan PR1.2 claims idle mode if on the phone! That's not nice, so this is a work around for that
    _emitRealIdle();

    // Note that PR1.2 also seems to return data usage as Cellular::SystemControl::UnknownActivity (-1)
    // and not as Cellular::SystemControl::Data as it should.
    if (_currentCellularActivity == Cellular::SystemControl::Idle && _pendingCellularMode >= 0) {
        qDebug("HarmattanPlatformUtil::cellularActivityChanged activating pending cellular mode");
        setCellularMode(_pendingCellularMode);
        _pendingCellularMode = -1;
    }
}

void
HarmattanPlatformUtil::privateSystemStateChanged(MeeGo::QmSystemState::StateIndication systemState)
{
    qDebug("HarmattanPlatformUtil::privateSystemStateChanged %d", systemState);
    if (systemState == MeeGo::QmSystemState::Shutdown) {
        // Doesn't feel safe to trigger actions in any other case.
        qDebug("HarmattanPlatformUtil::privateSystemStateChanged emitting shuttingDown signal");
        emit shuttingDown();
    }
}

void
HarmattanPlatformUtil::privateBatteryChargingStateChanged(MeeGo::QmBattery::ChargingState chargingStateEnum)
{
    qDebug("HarmattanPlatformUtil::privateChargingStateChanged %d", chargingStateEnum);
    int chargingState = -1;
    switch (chargingStateEnum) {
    case MeeGo::QmBattery::StateNotCharging:
        chargingState = 0;
        break;
    case MeeGo::QmBattery::StateCharging:
        chargingState = 1;
        break;
    default:
        qWarning("HarmattanPlatformUtil::privateChargingStateChanged unsupported chargingState %d", chargingState);
    }

    if (chargingState >= 0) {
        emit batteryChargingStateChanged(chargingState);
    }
}

ActionPresence *
HarmattanPlatformUtil::createActionPresence()
{
    return new ActionPresenceImpl();
}

