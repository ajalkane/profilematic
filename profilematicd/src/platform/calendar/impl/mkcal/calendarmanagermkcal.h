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
#ifndef CALENDARMANAGERMKCAL_H
#define CALENDARMANAGERMKCAL_H

#include <extendedcalendar.h>
#include <extendedstorage.h>

#include "../../calendarmanager.h"

class CalendarManagerMkCal: public CalendarManager
{
    mKCal::ExtendedCalendar::Ptr _calendarBackendPtr;
    mKCal::ExtendedStorage::Ptr _storagePtr;

    void _ensureCalendarInitialized();
    void _fixIncidenceDates(KCalCore::Incidence::Ptr incidence, QDateTime &dtStart, QDateTime &dtEnd);
public:
    CalendarManagerMkCal(QObject *parent = 0);

    QList<CalendarEntry> loadCalendarEntries(const QDate &startDate, const QDate &endDate);
};

#endif // CALENDARMANAGERMKCAL_H
