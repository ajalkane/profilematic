/**********************************************************************
 * Copyright 2011 Arto Jalkanen
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
#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QList>
#include <QSettings>

#include "model/rule.h"

class Settings
{
    static void _writeIntList(QSettings &s, const QString &prefix, const QString &key, const QList<int> &values);
    static void _readIntList(QSettings &s, const QString &prefix, const QString &key, QList<int> &values);

public:
    Settings();

    static void write(const QList<Rule> &rules);
    static void read(QList<Rule> &rules);


};

#endif // SETTINGS_H
