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

    /**
     * Loads calendar entries in the given date range
     *
     * The startDate is inclusive, and endDate exclusive. In practice
     * it means for date+time range this: (startDate):00:00 - (endDate):00:00.
     * As an example, if all calendar entries are wanted for one DAY, the parameters
     * should be (DAY), (DAY + 1day). For example: (DAY, DAY.addDays(1)).
     *
     * There is no restrictions on the order of returned calendar entries, although it
     * it is preferrable to return them sorted by (startDate, endDate)
     */
    virtual QList<CalendarEntry> loadCalendarEntries(const QDate &startDate, const QDate &endDate) = 0;

    /**
     * Closes the calendar, releasing any resources that might be used.
     */
    virtual void closeCalendar() = 0;
signals:
    /**
     * Emitted when any change done on calendar. If this signal is emitted, must assume any
     * entries loaded with loadCalendarEntries are invalid. This signal must be emitted only
     * if loadCalendarEntries() have been called, and closeCalendar() has not been called
     * after that.
     */
    void onCalendarChanged();
};

#endif // CALENDARMANAGER_H
