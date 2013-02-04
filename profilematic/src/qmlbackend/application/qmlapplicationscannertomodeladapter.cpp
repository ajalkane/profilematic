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
