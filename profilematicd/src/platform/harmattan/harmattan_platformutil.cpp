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
#include <QDebug>

#include "harmattan_platformutil.h"
#include "../../logic/presence/actionpresenceimpl.h"
#include <qmdevicemode.h>
#include <RadioAccess>
#include <QProcess>
#include <gconfitem.h>

#include "../calendar/impl/mkcal/calendarmanagermkcal.h"

#define GCONF_STAND_BY_SCREEN_KEY "/system/osso/dsm/display/use_low_power_mode"
#define GCONF_BACKGROUND_CONNECTIONS_DISABLED_KEY "/system/osso/connectivity/network_type/restricted_mode"
#define PLATFORMUTIL_APP "/opt/profilematic/bin/platformutil"
#define FLIGHT_MODE_CMD "invoker --type=m " PLATFORMUTIL_APP

#include <timed/interface>
#include <timed/event>

int testAlarm() {
    qDebug() << "Testing alarm";

    Maemo::Timed::Event event;
    event.setAttribute("APPLICATION", "ProfileMatic");
    event.setAttribute("TITLE", "Get up now!");
    event.setAttribute("PLUGIN", "libclockalarm");
    event.setBootFlag();
    event.setAlarmFlag();
    event.setReminderFlag();
    // set the alarm in 10s from now
    event.setTicker(QDateTime::currentDateTime().toTime_t() + 10);

    // timed interface setup
    Maemo::Timed::Interface timedIface;
    if(!timedIface.isValid()) {
       qDebug() << "Invalid timed interface:" << timedIface.lastError();
       return 1;
    }
    // add the event
    QDBusReply<uint> reply = timedIface.add_event_sync(event);
    if(!reply.isValid()) {
       qDebug() << "Adding event failed:" << reply.error().message();
       return 1;
    }
    qDebug() << "Added event with cookie:" << reply.value();
    return 0;
}

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

    // testAlarm();
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
        IFDEBUG(qDebug("HarmattanPlatformUtil::flightMode warning unrecognized deviceMode %d", mode));
    }

    IFDEBUG(qDebug("HarmattanPlatformUtil::flightMode current deviceMode %d", flightMode));
    return flightMode;
}

void
HarmattanPlatformUtil::setFlightMode(int flightMode) {
    IFDEBUG(qDebug("HarmattanPlatformUtil::Setting flightMode to %d", flightMode));
    const char *par = flightMode == 1 ? " flightModeOn" : " flightModeOff";

    QString cmd(FLIGHT_MODE_CMD);
    cmd += par;

    int ret = QProcess::execute(cmd);
    IFDEBUG(qDebug("HarmattanPlatformUtil::setFlightMode() exec return code %d", ret));
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
        IFDEBUG(qDebug("HarmattanPlatformUtil::psmState warning unrecognized psmState %d", state));
    }

    IFDEBUG(qDebug("HarmattanPlatformUtil::psmState current Power Saving State %d", psmState));
    return psmState;
}

void
HarmattanPlatformUtil::setPowerSavingMode(int state) {
    MeeGo::QmDeviceMode deviceMode;

    IFDEBUG(qDebug("HarmattanPlatformUtil::Setting Power Saving State to %d", state));
    if (state == 0 || state == 1) {
        bool return_value = deviceMode.setPSMState(state == 1
                                          ? MeeGo::QmDeviceMode::PSMStateOn
                                          : MeeGo::QmDeviceMode::PSMStateOff);

        IFDEBUG(qDebug("HarmattanPlatformUtil::Setting Power Saving State, return value %d", return_value));
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
        IFDEBUG(qDebug("HarmattanPlatformUtil::cellularMode warning unrecognized RadioAccess mode %d", cellularMode));
    }

    IFDEBUG(qDebug("HarmattanPlatformUtil::cellularMode current cellular mode %d", cellularMode));
    return cellularMode;
}

void
HarmattanPlatformUtil::setCellularMode(int cellularMode) {
    IFDEBUG(qDebug("HarmattanPlatformUtil::setCellularMode setting cellular mode %d", cellularMode));
    // Note that PR1.2 also seems to return data usage as Cellular::SystemControl::UnknownActivity (-1)
    // and not as Cellular::SystemControl::Data as it should.
    if (_currentCellularActivity != 0) {
        // Delay changing of cellular mode until no calls or data transfer. Otherwise
        // call/transfer aborts.
        IFDEBUG(qDebug("HarmattanPlatformUtil::setCellularMode cellularActivity is %d, pending the change of mode", _currentCellularActivity));
        _pendingCellularMode = cellularMode;
        return;
    }
    Cellular::RadioAccess radioAccess;
    Cellular::RadioAccess::Mode mode = Cellular::RadioAccess::Unknown;
    switch (cellularMode) {
    case 0:
        IFDEBUG(qDebug("HarmattanPlatformUtil::setCellularMode setting Dual Mode"));
        mode = Cellular::RadioAccess::DualMode;
        break;
    case 1:
        IFDEBUG(qDebug("HarmattanPlatformUtil::setCellularMode setting 2G Mode"));
        mode = Cellular::RadioAccess::OnlyTwoG;
        break;
    case 2:
        IFDEBUG(qDebug("HarmattanPlatformUtil::setCellularMode setting 3G Mode"));
        mode = Cellular::RadioAccess::OnlyThreeG;
        break;
    default:
        IFDEBUG(qWarning("HarmattanPlatformUtil::setCellularMode warning unrecognized mode %d", cellularMode));
    }

    _pendingCellularMode = -1;

    if (mode != Cellular::RadioAccess::Unknown) {
        radioAccess.setMode(mode);
    }
}

int
HarmattanPlatformUtil::cellularActivity() const {
    int activity = (int)_cellularControl.activity();
    IFDEBUG(qDebug("HarmattanPlatformUtil::cellularActivity %d", activity));
    return activity;
}

void
HarmattanPlatformUtil::setStandByScreenMode(int mode) {
    IFDEBUG(qDebug("HarmattanPlatformUtil::setStandByScreenMode %d", mode));
    GConfItem gconfItem(GCONF_STAND_BY_SCREEN_KEY);
    if (mode == 0 || mode == 1) {
        gconfItem.set(mode);
    } else {
        IFDEBUG(qDebug("HarmattanPlatformUtil::setStandByScreenMode unsupported mode %d", mode));
    }
}

int
HarmattanPlatformUtil::standByScreenMode() const {
    GConfItem gconfItem(GCONF_STAND_BY_SCREEN_KEY);
    return gconfItem.value().toInt();
}


void
HarmattanPlatformUtil::setBackgroundConnectionsMode(int mode) {
    IFDEBUG(qDebug("HarmattanPlatformUtil::setBackgroundConnectionsMode %d", mode));
    GConfItem gconfItem(GCONF_BACKGROUND_CONNECTIONS_DISABLED_KEY);
    if (mode == 0 || mode == 1) {
        gconfItem.set((mode + 1) % 2);
    } else {
        IFDEBUG(qDebug("HarmattanPlatformUtil::setBackgroundConnectionsMode unsupported mode %d", mode));
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
    IFDEBUG(qDebug("HarmattanPlatformUtil::batteryChargingState"));
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
    IFDEBUG(qDebug("HarmattanPlatformUtil::_emitRealIdle: a(%d) c(%d) i(%d)", _currentActivity, _currentCellularActivity, _currentIdle));

    // Harmattan PR1.2 claims idle mode if on the phone! That's not nice, so this is a work around for that.
    // Basically not signaling idle if call is in progress.
    // "RealIdle" is designed to fulfill following special cases:
    // 1) If call is on-going, will not go from active to "RealIdle"
    // 2) If idle and call incoming, will not exit "RealIdle".
    // The reasoning for case 2) is: if idle is used for example as a "night time" rule to keep phone silent,
    // you don't want to exit "realIdle" because someone tries calling. So realIdle is only exited
    // if idle is active.
    if (_currentIdle == false && (_currentCellularActivity != Cellular::SystemControl::Call && _currentActivity == MeeGo::QmActivity::Inactive)) {
        _currentIdle = true;
        emit userActivityIdleChanged(true);
    } else if (_currentIdle == true && (_currentActivity == MeeGo::QmActivity::Active)) {
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
    IFDEBUG(qDebug("HarmattanPlatformUtil::cellularActivityChanged %d", activity));
    emit cellularActivityChanged(activity);

    // Harmattan PR1.2 claims idle mode if on the phone! That's not nice, so this is a work around for that
    _emitRealIdle();

    // Note that PR1.2 also seems to return data usage as Cellular::SystemControl::UnknownActivity (-1)
    // and not as Cellular::SystemControl::Data as it should.
    if (_currentCellularActivity == Cellular::SystemControl::Idle && _pendingCellularMode >= 0) {
        IFDEBUG(qDebug("HarmattanPlatformUtil::cellularActivityChanged activating pending cellular mode"));
        setCellularMode(_pendingCellularMode);
    }
}

void
HarmattanPlatformUtil::privateSystemStateChanged(MeeGo::QmSystemState::StateIndication systemState)
{
    IFDEBUG(qDebug("HarmattanPlatformUtil::privateSystemStateChanged %d", systemState));
    if (systemState == MeeGo::QmSystemState::Shutdown) {
        // Doesn't feel safe to trigger actions in any other case.
        IFDEBUG(qDebug("HarmattanPlatformUtil::privateSystemStateChanged emitting shuttingDown signal"));
        emit shuttingDown();
    }
}

void
HarmattanPlatformUtil::privateBatteryChargingStateChanged(MeeGo::QmBattery::ChargingState chargingStateEnum)
{
    IFDEBUG(qDebug("HarmattanPlatformUtil::privateChargingStateChanged %d", chargingStateEnum));
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

int
HarmattanPlatformUtil::batteryLevel() const {
    int level = _qmbattery.getRemainingCapacityPct();
    IFDEBUG(qDebug("HarmattanPlatformUtil::batteryLevel returning %d", level));
    return level;
}

void
HarmattanPlatformUtil::monitorBatteryLevel(bool monitor ) {
    IFDEBUG(qDebug("HarmattanPlatformUtil::monitorBatteryLevel monitor: %d", monitor));
    if (monitor) {
        connect(&_qmbattery, SIGNAL(batteryRemainingCapacityChanged(int,int)), this, SLOT(batteryRemainingCapacityChanged(int,int)), Qt::UniqueConnection);
    } else {
        disconnect(&_qmbattery, SIGNAL(batteryRemainingCapacityChanged(int,int)), this, SLOT(batteryRemainingCapacityChanged(int,int)));
    }
}

void
HarmattanPlatformUtil::batteryRemainingCapacityChanged(int percentage, int bars) {
    IFDEBUG(qDebug("HarmattanPlatformUtil::batteryRemainingCapacityChanged pct %d bars %d", percentage, bars));
    emit batteryLevelChanged(percentage);
}

void
HarmattanPlatformUtil::scheduleAlarm(const RuleActionAlarm &alarm) {
    // Mostly copied from example given by Cristi Boian here: http://www.cristiboian.com/2012/05/setting-alarms-in-harmattan-nokia-n9.html
    IFDEBUG(qDebug() << "HarmattanPlatformUtil::scheduleAlarm title" << alarm.getTitle()
            << "alarmInSeconds" << alarm.getAlarmInSeconds()
            << "snoozeInMinutes" << alarm.getSnoozeInMinutes());

    Maemo::Timed::Event event;
    uint ticker = QDateTime::currentDateTime().toTime_t() + alarm.getAlarmInSeconds();
    // Hardcoded for Harmattan default 10 minutes default - seems like the real default is hard
    // to get and hart to set by user. So settle for 10 minutes
    int snoozeInMinutes = alarm.getSnoozeInMinutes() > 0 ? alarm.getSnoozeInMinutes() : 10;

    // event.setAttribute("APPLICATION", "ProfileMatic");
    // Use clock as application so that the alarm's appear in Harmattan's Clock app
    // Having this as "clock" allows the alarm to be deleted and disabled from clock.
    event.setAttribute("APPLICATION", "clock");
    event.setAttribute("TITLE", alarm.getTitle());
    event.setAttribute("PLUGIN", "libclockalarm");
    event.setAttribute("alarmtime", QDateTime::fromTime_t(ticker).time().toString("hh:mm"));
    event.setAttribute("enabled", QString::number(1));
    // Hardcoded for Harmattan default 10 minutes if not set - this has no effect it seems
    event.setAttribute("snooze", QString::number(snoozeInMinutes));
    event.setAttribute("trigger", QString::number(ticker));
    if (!alarm.getSound().trimmed().isEmpty()) {
        IFDEBUG(qDebug() << "HarmattanPlatformUtil::scheduleAlarm setting alarm sound to" << alarm.getSound());
        event.setAttribute("sound", alarm.getSound().trimmed());
    } else {
        IFDEBUG(qDebug() << "HarmattanPlatformUtil::scheduleAlarm not setting alarm sound");
        // Seems like the clock application doesn't like missing attributes - without settings something
        // here you can't disable the alarm from clock application. Empty is also not allowed, but
        // using an unexisting sound file will result in default sound.
        event.setAttribute("sound", QString("UNDEFINED"));
    }

    // Keeps the alarm in Clock application so that user can modify it. This creates multiple entries
    // unless specifically deleted, so won't be used. It'd be optimal if we could query if an alarm
    // with specified title is already there and use it. But the API does not allow that, even though
    // you could probably do it with DBus calls.
    // event.setKeepAliveFlag();

    // These are needed anyway, even as we set the "snooze" flag above. The first
    // button is snooze, so that's what the addButton seems to be attached to.
    // Not too intuitive, but seems to work.
    Maemo::Timed::Event::Button &b1 = event.addButton() ;

    if (snoozeInMinutes > 0) {
        b1.setSnooze(snoozeInMinutes * 60);
    } else {
        b1.setSnoozeDefault();
    }

    event.setBootFlag();
    event.setAlarmFlag();
    event.setReminderFlag();

    // Try to hide the snooze button - does not work
    //event.hideSnoozeButton1();
    // set the alarm in given amount of seconds from now - at least this seems to work
    event.setTicker(ticker);

    // timed interface setup
    Maemo::Timed::Interface timedIface;    
    if(!timedIface.isValid()) {
       IFDEBUG(qDebug() << "HarmattanPlatformUtil::scheduleAlarm Invalid timed interface:" << timedIface.lastError());
       return;
    }
    // add the event
    QDBusReply<uint> reply = timedIface.add_event_sync(event);
    if(!reply.isValid()) {
       IFDEBUG(qDebug() << "HarmattanPlatformUtil::scheduleAlarm Adding event failed:" << reply.error().message());
       return;
    }
    IFDEBUG(qDebug() << "HarmattanPlatformUtil::scheduleAlarm Added event with cookie18" << reply.value());
}

CalendarManager *
HarmattanPlatformUtil::createCalendarManager(QObject *parent)
{
    return new CalendarManagerMkCal(parent);
}
