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
#include "qmlnfcmobility.h"

#include <QNearFieldTarget>

QmlNfcMobility::QmlNfcMobility(QObject *parent)
    : QObject(parent)
{
    _nfcManager = new QNearFieldManager(this);

    qDebug("QmlNfcMobility isAvailable: %d", _nfcManager->isAvailable());
}

void
QmlNfcMobility::monitorNfc(bool monitor) {
    if (monitor) {
        connect(_nfcManager, SIGNAL(targetDetected(QNearFieldTarget *)), this, SLOT(targetDetected(QNearFieldTarget *)), Qt::UniqueConnection);
        _nfcManager->startTargetDetection();
    } else {
        _nfcManager->stopTargetDetection();
        disconnect(_nfcManager, SIGNAL(targetDetected(QNearFieldTarget *)), this, SLOT(targetDetected(QNearFieldTarget *)));
    }
}

void
QmlNfcMobility::targetDetected(QNearFieldTarget *target) {
    qDebug("QmlNfcMobility::targetDetected()");
    QString uidStr(target->uid().toHex());
    qDebug("QmlNfcMobility::targetDetected() uid: %s", qPrintable(uidStr));
    emit currentNfcUidChanged(uidStr);
    target->deleteLater();
}

bool
QmlNfcMobility::isAvailable() const {
    return _nfcManager->isAvailable();
}
