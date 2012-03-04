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

#include "../platformutil.h"

class HarmattanPlatformUtil : public PlatformUtil
{
    Q_OBJECT

public:
    HarmattanPlatformUtil(QObject *parent = 0);
    virtual ~HarmattanPlatformUtil();

    virtual int flightMode() const;
    virtual void setFlightMode(int flightMode);
    virtual int powerSavingMode() const;
    virtual void setPowerSavingMode(int state);
    virtual int cellularMode() const;
    virtual void setCellularMode(int state);

    virtual void publishNotification(const QString &message);

    ActionPresence *createActionPresence();
};

#endif // PLATFORMUTIL_H
