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

signals:
    void userActivityIdleChanged(bool isIdle);
    // -1: Unknown, 0: Idle, 1: Signaling, 2: Call, 3: Data
    void cellularActivityChanged(int activity);
    void shuttingDown();
    void batteryChargingStateChanged(int chargingState);
};

#endif // PLATFORMUTIL_H
