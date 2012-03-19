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
protected:
    PlatformUtil(QObject *parent = 0);
public:
    static PlatformUtil *create();
    virtual ~PlatformUtil();

    virtual int flightMode() const;
    // 0: flight mode off, 1: flight mode on
    virtual void setFlightMode(int flightMode);
    // 0: Power Saving Mode off, 1: Power Saving mode on
    virtual int powerSavingMode() const;
    virtual void setPowerSavingMode(int state);
    // 0: Dual mode, 1: 2G only, 2: 3G only
    virtual int cellularMode() const;
    virtual void setCellularMode(int state);
    virtual void publishNotification(const QString &message);
    // 0: standByScreenMode off: 0, on: 1: on
    virtual int standByScreenMode() const;
    virtual void setStandByScreenMode(int mode);

    virtual ActionPresence *createActionPresence();
};

#endif // PLATFORMUTIL_H
