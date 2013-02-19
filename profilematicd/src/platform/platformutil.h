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
#ifndef PLATFORMUTIL_H
#define PLATFORMUTIL_H

#include <QObject>

#include "../model/rule.h"
// IMPROVE it's wrong to have in platform util this... rather platformutil should abstract the QtTelepathy functions for portability between platforms
#include "../logic/presence/actionpresence.h"
#include "calendar/calendarmanager.h"

class ActionPresence;

class PlatformUtil : public QObject
{
    Q_OBJECT

    static PlatformUtil *_instance;

    static PlatformUtil *_create();
protected:
    PlatformUtil(QObject *parent = 0);
public:
    // Initialize must be called before instance can be used.
    static inline PlatformUtil *instance() {
        return _instance;
    }

    static void initialize();
    // Only intended to be used Unit Tests to initialize a mock instance
    static void initialize(PlatformUtil *useInstance);
    static void deinitialize();

    virtual ~PlatformUtil();

    virtual int flightMode() const;
    // 0: flight mode off, 1: flight mode on
    virtual void setFlightMode(int flightMode);
    // 0: Power Saving Mode off, 1: Power Saving mode on
    virtual int powerSavingMode() const;
    virtual void setPowerSavingMode(int state);
    // Returns true if daemon has credentials to change flightMode and powerSavingMode
    virtual bool hasDeviceModeCredential() const;

    // 0: Dual mode, 1: 2G only, 2: 3G only
    virtual int cellularMode() const;
    virtual void setCellularMode(int state);
    // -1: Unknown, 0: Idle, 1: Signaling, 2: Call, 3: Data
    virtual int cellularActivity() const;
    virtual void publishNotification(const QString &message);
    // 0: standByScreenMode off: 0, 1: on
    virtual int standByScreenMode() const;
    virtual void setStandByScreenMode(int mode);
    // 0: background connections off, 1: on
    virtual int backgroundConnectionsMode() const;
    virtual void setBackgroundConnectionsMode(int mode);    
    // 0: Not charging, 1: Charging
    virtual int batteryChargingState() const;
    virtual ActionPresence *createActionPresence();

    virtual bool isUserActivityIdle();

    // Returns the battery level in percents between 0-100
    virtual int batteryLevel() const;
    // IMPROVE: Qt has ways to detect if any object is connected to signal, that would be nicer way to
    // do this and would make this method unnecessary.
    virtual void monitorBatteryLevel(bool monitor);

    // If snoozeInMinutes is 0 or less, the platform default snooze time should be used (if makes sense)
    virtual void scheduleAlarm(const RuleActionAlarm &alarm);

    virtual CalendarManager *createCalendarManager(QObject *parent = 0);

    // Returns the device volume in percents between 0-100
    virtual int deviceVolume() const;
    virtual void setDeviceVolume(int deviceVolume);

signals:
    void userActivityIdleChanged(bool isIdle);
    // -1: Unknown, 0: Idle, 1: Signaling, 2: Call, 3: Data
    void cellularActivityChanged(int activity);
    void shuttingDown();
    void batteryChargingStateChanged(int chargingState);
    void batteryLevelChanged(int batteryLevel);
};

#endif // PLATFORMUTIL_H
