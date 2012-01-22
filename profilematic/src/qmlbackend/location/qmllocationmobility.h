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
#ifndef QMLLOCATIONMOBILITY_H
#define QMLLOCATIONMOBILITY_H

#include <QSystemNetworkInfo>

QTM_USE_NAMESPACE

#include "../qmllocation.h"

// Location services for platforms that support QtMobility
class QmlLocationMobility : public QmlLocation
{
    Q_OBJECT

    QSystemNetworkInfo *_source;
public:
    QmlLocationMobility(QObject *parent = 0);
    ~QmlLocationMobility();

    virtual int getCurrentCell() const;

public slots:
    void monitorCellIdChange(bool monitor);
};

#endif // QMLLOCATIONMOBILITY_H
