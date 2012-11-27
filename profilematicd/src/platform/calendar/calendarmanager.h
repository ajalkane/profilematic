/**********************************************************************
 * Copyright 2012 Arto Jalkanen
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
#ifndef CALENDARMANAGER_H
#define CALENDARMANAGER_H

#include <QObject>
#include <QList>
#include <QDate>

#include "calendarentry.h"

class CalendarManager : public QObject
{
    Q_OBJECT

public:
    CalendarManager(QObject *parent = 0);
    virtual ~CalendarManager();

    virtual QList<CalendarEntry> loadCalendarEntries(const QDate &startDate, const QDate &endDate) = 0;

signals:
    void onCalendarChanged();
};

#endif // CALENDARMANAGER_H
