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
#ifndef QMLAPPLICATIONSCANNERTOMODELADAPTER_H
#define QMLAPPLICATIONSCANNERTOMODELADAPTER_H

#include <QObject>

#include "qmlapplicationsmodel.h"
#include "qmlapplicationsscanner.h"

/**
 * This class is modeled so that it will be easy to make scanning threaded.
 * But it seems at the moment that scanning of applications is fast enough
 * that thread (and Spinner for UI) is not needed.
 */
class QmlApplicationScannerToModelAdapter : public QObject
{
    Q_OBJECT

    QmlApplicationsModel *_model;
    QmlApplicationsScanner *_scanner;

    bool _isScanning;

    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged)

public:
    QmlApplicationScannerToModelAdapter(QmlApplicationsModel *model, QmlApplicationsScanner *scanner);

    // Asynchronous interface (not implemented)
    Q_INVOKABLE void startScan();
    Q_INVOKABLE void stopScan();
    inline bool isScanning() const { return _isScanning; }

    // Synchronous interface.
    Q_INVOKABLE void scan();
signals:
    void isScanningChanged();

private slots:
    void applicationScanned(const QString &appName, const QString &launcher, const QString &iconUri);
};

#endif // QMLAPPLICATIONSCANNERTOMODELADAPTER_H
