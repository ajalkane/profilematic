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
#ifndef CALENDARENTRY_H
#define CALENDARENTRY_H

#include <QDateTime>
#include <QString>

class CalendarEntry
{
    QDateTime _start;
    QDateTime _end;
    QString _summary;

public:
    CalendarEntry(const QDateTime &start, const QDateTime &end, const QString &summary);

    inline const QDateTime &start()   const { return _start; }
    inline const QDateTime &end()     const { return _end; }
    inline const QString   &summary() const { return _summary; }
};

#endif // CALENDARENTRY_H
