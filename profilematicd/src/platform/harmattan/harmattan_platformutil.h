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
#ifndef HARMATTAN_PLATFORMUTIL_H
#define HARMATTAN_PLATFORMUTIL_H

#include <qmactivity.h>
#include <qmbattery.h>
#include <qmsystemstate.h>
#include <SystemControl>

#include "../platformutil.h"

class HarmattanPlatformUtil : public PlatformUtil
{
    Q_OBJECT

    MeeGo::QmBattery _qmbattery;
    MeeGo::QmActivity _qmActivity;
    MeeGo::QmActivity::Activity _currentActivity;
    MeeGo::QmSystemState _systemState;

    bool _currentIdle;

    Cellular::SystemControl _cellularControl;
    Cellular::SystemControl::Activity _currentCellularActivity;
    int _pendingCellularMode;

    void _emitRealIdle();
public:
    HarmattanPlatformUtil(QObject *parent = 0);
    virtual ~HarmattanPlatformUtil();

    virtual int flightMode() const;
    virtual void setFlightMode(int flightMode);
    virtual int powerSavingMode() const;
    virtual void setPowerSavingMode(int state);
    virtual bool hasDeviceModeCredential() const;
    virtual int cellularMode() const;
    virtual int cellularActivity() const;
    virtual void setCellularMode(int state);
    virtual void publishNotification(const QString &message);
    virtual int standByScreenMode() const;
    virtual void setStandByScreenMode(int mode);
    virtual int backgroundConnectionsMode() const;
    virtual void setBackgroundConnectionsMode(int mode);
    virtual int batteryChargingState() const;

    ActionPresence *createActionPresence();

    virtual bool isUserActivityIdle();

    virtual int batteryLevel() const;
    virtual void monitorBatteryLevel(bool monitor);

private slots:
    void activityChanged(MeeGo::QmActivity::Activity activity);
    void privateCellularActivityChanged(int activity);
    void privateSystemStateChanged(MeeGo::QmSystemState::StateIndication);
    void privateBatteryChargingStateChanged(MeeGo::QmBattery::ChargingState chargingState);
};
#endif // HARMATTAN_PLATFORMUTIL_H
