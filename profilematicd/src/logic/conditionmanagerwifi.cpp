#include "conditionmanagerwifi.h"

ConditionManagerWifi::ConditionManagerWifi(QObject *parent)
    : ConditionManager(parent)
{
    _networkConfigurationManager = new QNetworkConfigurationManager(this);
    monitorConfiguration(true);

    QList<QNetworkConfiguration> confs = _networkConfigurationManager->allConfigurations();
    QList<QNetworkConfiguration>::const_iterator i = confs.constBegin();
    qDebug("All Network configurations");
    for (; i != confs.constEnd(); ++i) {
        _logConfiguration(*i);
    }
}

ConditionManagerWifi::~ConditionManagerWifi() {
}

void
ConditionManagerWifi::_logConfiguration(const QNetworkConfiguration &config) const {
    qDebug("QNetworkConfiguration::bearerTypeName %s", qPrintable(config.bearerTypeName()));
    qDebug("QNetworkConfiguration::identifier     %s", qPrintable(config.identifier()));
    qDebug("QNetworkConfiguration::name           %s", qPrintable(config.name()));
    qDebug("QNetworkConfiguration::state          %0x", (int)config.state());
    qDebug(" ");
}

void
ConditionManagerWifi::startRefresh() {}

bool
ConditionManagerWifi::refresh(const Rule &rule) {
    return true;
}

void
ConditionManagerWifi::matchedRule(const Rule &rule) {}

void
ConditionManagerWifi::endRefresh() {}

void
ConditionManagerWifi::monitorConfiguration(bool monitor) {
    if (monitor) {
        connect(_networkConfigurationManager,
                SIGNAL(configurationAdded(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationAdded(const QNetworkConfiguration &)));
    }
}

void
ConditionManagerWifi::onConfigurationAdded(const QNetworkConfiguration   &config) {
    qDebug("Network configuration added");
    _logConfiguration(config);
}
void
ConditionManagerWifi::onConfigurationChanged(const QNetworkConfiguration &config) {
    qDebug("Network configuration changed");
    _logConfiguration(config);
}

void
ConditionManagerWifi::onConfigurationRemoved(const QNetworkConfiguration &config) {
    qDebug("Network configuration removed");
    _logConfiguration(config);
}
