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

#include <QTimer>

#include <extendedcalendar.h>
#include <extendedstorage.h>

#include "../../calendarmanager.h"

class CalendarManagerMkCal: public CalendarManager, public mKCal::ExtendedStorageObserver
{
    Q_OBJECT

    mKCal::ExtendedCalendar::Ptr _calendarBackendPtr;
    mKCal::ExtendedStorage::Ptr _storagePtr;

    /**
      * Timer to close the calendar backend after a certain period of time, to free memory.
      */
    QTimer _inactivityCalendarCloseTimer;

    void _ensureCalendarInitialized();
    void _fixIncidenceDates(KCalCore::Incidence::Ptr incidence, QDateTime &dtStart, QDateTime &dtEnd);

public:
    CalendarManagerMkCal(QObject *parent = 0);

    QList<CalendarEntry> loadCalendarEntries(const QDate &startDate, const QDate &endDate);
    void closeCalendar();

    // ExtendedStorageObserver
    void storageModified(mKCal::ExtendedStorage *storage, const QString& info);
    void storageProgress(mKCal::ExtendedStorage *storage, const QString &info);
    void storageFinished(mKCal::ExtendedStorage *storage, bool error, const QString &info);

private slots:
    void _reopenCalendar();

};

#endif // CALENDARMANAGERMKCAL_H
