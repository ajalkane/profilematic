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
#ifndef QMLNFCMOBILITY_H
#define QMLNFCMOBILITY_H

#include <QObject>

#include <QNearFieldManager>

QTM_USE_NAMESPACE

class QmlNfcMobility : public QObject
{
    Q_OBJECT

    QNearFieldManager *_nfcManager;

public:
    QmlNfcMobility(QObject *parent = 0);

private slots:
    void targetDetected(QNearFieldTarget *target);

public slots:
    void monitorNfc(bool monitor);

signals:
    void currentNfcUidChanged(const QString &uid) const;
};

#endif // QMLNFCMOBILITY_H
