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
#include <QDebug>

#include "qmlapplicationscannertomodeladapter.h"

QmlApplicationScannerToModelAdapter::QmlApplicationScannerToModelAdapter(QmlApplicationsModel *model, QmlApplicationsScanner *scanner)
    : _model(model), _scanner(scanner), _isScanning(false)
{
    connect(_scanner, SIGNAL(applicationScanned(const QString &,const QString &,const QString &)),
            this, SLOT(applicationScanned(const QString &,const QString &,const QString &)));
}

void
QmlApplicationScannerToModelAdapter::applicationScanned(const QString &appName, const QString &launcher, const QString &iconUri) {
    qDebug() << "QmlApplicationScannerToModelAdapter::applicationScanned with icon " << iconUri;

    QmlApplication app;
    app.setApplicationLauncher(launcher);
    app.setApplicationName(appName);
    app.setIconUri(iconUri);
    _model->addApplication(app);
}

void
QmlApplicationScannerToModelAdapter::scan() {
    _model->clearApplications();
    _scanner->scan();
}

void
QmlApplicationScannerToModelAdapter::startScan() {
    //    if (_isScanning == false) {
    //        _isScanning
    //        emit isScanningChanged();
    //        emit
    //    }
}

void
QmlApplicationScannerToModelAdapter::stopScan() {

}
