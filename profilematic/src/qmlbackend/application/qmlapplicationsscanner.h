/**********************************************************************
 * Copyright 2013 Arto Jalkanen
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
#ifndef QMLAPPLICATIONSSCANNER_H
#define QMLAPPLICATIONSSCANNER_H

#include <QObject>

#include "qmlapplication.h"

class QmlApplicationsScanner : public QObject
{
    Q_OBJECT

    void _scanFolder(const QString &path);
public:
    QmlApplicationsScanner(QObject *parent = 0);

public slots:
    void scan();

    QmlApplication applicationFromLauncher(const QString &launcher);

signals:
    void applicationScanned(const QString &appName, const QString &launcher, const QString &iconUri);
};

#endif // QMLAPPLICATIONSSCANNER_H
