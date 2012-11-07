#include "qmlnetworkinfo.h"

QmlNetworkInfo::QmlNetworkInfo(QObject *parent)
    : QObject(parent),
      _monitorNameChanges(false),
      _monitorModeChanges(false),
      _monitorStatusChanges(false),
      _networkMode(UnknownMode)
{
    qDebug("QmlNetworkInfo::constructor() this hate machine has been created");
    _networkConfigurationManager = new QNetworkConfigurationManager(this);
    QNetworkConfiguration conf = _getCurrentActiveConfiguration();
    _networkMode = _networkConfigurationToMode(conf);
    _networkName = conf.name();
    _networkStatus = _networkConfigurationToStatus(conf);

    connect(_networkConfigurationManager,
            SIGNAL(configurationChanged(const QNetworkConfiguration &)),
            this, SLOT(onConfigurationChanged(const QNetworkConfiguration &)));
}

QNetworkConfiguration
QmlNetworkInfo::_getCurrentActiveConfiguration() const {
    qDebug("QmlNetworkInfo::_getCurrentActiveConfiguration");
    QList<QNetworkConfiguration> confs = _networkConfigurationManager->allConfigurations(QNetworkConfiguration::Active);
    foreach (const QNetworkConfiguration nc, confs) {
        if (nc.state() == QNetworkConfiguration::Active) {
            return nc;
        }
    }

    qDebug("QmlNetworkInfo::_getCurrentActiveConfiguration: no active configuration found, returning default");
    return _networkConfigurationManager->defaultConfiguration();
}

QmlNetworkInfo::Mode
QmlNetworkInfo::mode() const {
    return _networkMode;
}

void
QmlNetworkInfo::setMode(const Mode mode) {
    qDebug("QmlNetworkInfo::setMode %d", mode);
    _networkMode = mode;
    emit modeChanged();
}

QString
QmlNetworkInfo::networkName() const {
    return _networkName;
}

QString
QmlNetworkInfo::networkStatus() const {
    return _networkStatus;
}

bool
QmlNetworkInfo::monitorNameChanges() const {
    qDebug("QmlNetworkInfo::monitorNameChanges");    
    return _monitorNameChanges;
}

void
QmlNetworkInfo::setMonitorNameChanges(bool monitor) {
    qDebug("QmlNetworkInfo::setMonitorNameChanges %d", monitor);
    _monitorNameChanges = monitor;
}

bool
QmlNetworkInfo::monitorModeChanges() const {
    qDebug("QmlNetworkInfo::monitorNameChanges");
    return _monitorModeChanges;
}

void
QmlNetworkInfo::setMonitorModeChanges(bool monitor) {
    qDebug("QmlNetworkInfo::setMonitorModeChanges %d", monitor);
    _monitorModeChanges = monitor;
}

bool
QmlNetworkInfo::monitorStatusChanges() const {
    qDebug("QmlNetworkInfo::monitorStatusChanges");
    return _monitorStatusChanges;
}

void
QmlNetworkInfo::setMonitorStatusChanges(bool monitor) {
    qDebug("QmlNetworkInfo::setMonitorStatusChanges %d", monitor);
    _monitorStatusChanges = monitor;
}

void
logConfiguration(const QNetworkConfiguration &config) {
    qDebug("QmlNetworkInfo::bearerTypeName %s", qPrintable(config.bearerTypeName()));
    qDebug("QmlNetworkInfo::identifier     %s", qPrintable(config.identifier()));
    qDebug("QmlNetworkInfo::name           %s", qPrintable(config.name()));
    qDebug("QmlNetworkInfo::state          %0x", (int)config.state());
    qDebug(" ");
}

QmlNetworkInfo::Mode
QmlNetworkInfo::_networkConfigurationToMode(const QNetworkConfiguration &nc) {
    qDebug("QmlNetworkInfo::_networkConfigurationToMode: %d", nc.bearerType());
    if (nc.bearerType() == QNetworkConfiguration::BearerWLAN) {
        return QmlNetworkInfo::WlanMode;
    }
    qDebug("QmlNetworkInfo::_networkConfigurationToMode: returning unknown mode");

    return QmlNetworkInfo::UnknownMode;
}

QString
QmlNetworkInfo::_networkConfigurationToStatus(const QNetworkConfiguration &nc) {
    qDebug("QmlNetworkInfo::_networkConfigurationToState: %d", (int)nc.state());
    if (nc.state() == QNetworkConfiguration::Active) {
        return "Connected";
    }
    qDebug("QmlNetworkInfo::_networkConfigurationToState: returning unknown Undefined");

    return "Undefined";
}

void
QmlNetworkInfo::onConfigurationChanged(const QNetworkConfiguration &config) {
    qDebug("QmlNetworkInfo::Network configuration changed");
    logConfiguration(config);

    Mode mode = _networkConfigurationToMode(config);
    if (mode == _networkMode) {
        if (config.state() == QNetworkConfiguration::Active) {
            if (_monitorNameChanges && config.name() != _networkName) {
                _networkName = config.name();
                emit nameChanged();
            }
            if (_monitorStatusChanges) {
                QString status = _networkConfigurationToStatus(config);
                if (status != _networkStatus) {
                    _networkStatus = status;
                    emit statusChanged();
                }
            }
        }
        else {
            _networkName.clear();
            _networkStatus.clear();
            emit nameChanged();
            emit statusChanged();
        }
    }
}
