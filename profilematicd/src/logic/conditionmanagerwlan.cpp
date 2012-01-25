#include "conditionmanagerwlan.h"

ConditionManagerWlan::ConditionManagerWlan(QObject *parent)
    : ConditionManager(parent)
{
    _networkConfigurationManager = new QNetworkConfigurationManager(this);
    _networkInfo = new QSystemNetworkInfo(this);

    monitorConfiguration(true);

    QList<QNetworkConfiguration> confs = _networkConfigurationManager->allConfigurations();
    QList<QNetworkConfiguration>::const_iterator i = confs.constBegin();
    qDebug("All Network configurations");
    for (; i != confs.constEnd(); ++i) {
        _logConfiguration(*i);
    }
}

ConditionManagerWlan::~ConditionManagerWlan() {
}

void
ConditionManagerWlan::_logConfiguration(const QNetworkConfiguration &config) const {
    qDebug("QNetworkConfiguration::bearerTypeName %s", qPrintable(config.bearerTypeName()));
    qDebug("QNetworkConfiguration::identifier     %s", qPrintable(config.identifier()));
    qDebug("QNetworkConfiguration::name           %s", qPrintable(config.name()));
    qDebug("QNetworkConfiguration::state          %0x", (int)config.state());
    qDebug(" ");
}

void
ConditionManagerWlan::startRefresh() {}

bool
ConditionManagerWlan::refresh(const Rule &rule) {
    return true;
}

void
ConditionManagerWlan::matchedRule(const Rule &rule) {}

void
ConditionManagerWlan::endRefresh() {}

void
ConditionManagerWlan::monitorConfiguration(bool monitor) {
    if (monitor) {
        connect(_networkConfigurationManager,
                SIGNAL(configurationAdded(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationAdded(const QNetworkConfiguration &)));
        connect(_networkConfigurationManager,
                SIGNAL(configurationChanged(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationChanged(const QNetworkConfiguration &)));
        connect(_networkConfigurationManager,
                SIGNAL(configurationRemoved(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationRemoved(const QNetworkConfiguration &)));
        connect(_networkConfigurationManager,
                SIGNAL(configurationAdded(QNetworkConfiguration)),
                this, SLOT(onConfigurationAdded(QNetworkConfiguration)));
        connect(_networkConfigurationManager,
                SIGNAL(configurationChanged(QNetworkConfiguration)),
                this, SLOT(onConfigurationChanged(QNetworkConfiguration)));
        connect(_networkConfigurationManager,
                SIGNAL(configurationRemoved(QNetworkConfiguration)),
                this, SLOT(onConfigurationRemoved(QNetworkConfiguration)));

        connect(_networkInfo, SIGNAL(networkNameChanged(QSystemNetworkInfo::NetworkMode,QString)),
                this, SLOT(onNetworkNameChanged(QSystemNetworkInfo::NetworkMode,QString)));
    }
}

void
ConditionManagerWlan::onConfigurationAdded(const QNetworkConfiguration   &config) {
    qDebug("Network configuration added");
    _logConfiguration(config);
}
void
ConditionManagerWlan::onConfigurationChanged(const QNetworkConfiguration &config) {
    qDebug("Network configuration changed");
    _logConfiguration(config);
}

void
ConditionManagerWlan::onConfigurationRemoved(const QNetworkConfiguration &config) {
    qDebug("Network configuration removed");
    _logConfiguration(config);
}

void
ConditionManagerWlan::onNetworkNameChanged(QSystemNetworkInfo::NetworkMode mode, const QString & name) {
    qDebug("onNetworkNameChanged, mode %d, name %s", mode, qPrintable(name));
}
