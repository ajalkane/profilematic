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
#include "qmllocation.h"

#ifndef PROFILEMATIC_SIMULATED_CELLID
#include "location/qmllocationmobility.h"
#else
#include "location/qmllocationrandom.h"
#endif

QmlLocation::QmlLocation(QObject *parent)
    : QObject(parent)
{
}

QmlLocation *
QmlLocation::create() {
#ifndef PROFILEMATIC_SIMULATED_CELLID
    return new QmlLocationMobility;
#else
    return new QmlLocationRandom;
#endif
}

QmlLocation::~QmlLocation()
{
}
