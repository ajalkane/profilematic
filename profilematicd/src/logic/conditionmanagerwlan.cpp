#include "conditionmanagerwlan.h"

ConditionManagerWlan::ConditionManagerWlan(QObject *parent)
    : ConditionManager(parent), _requestRefreshEnabled(false)
{
    _networkConfigurationManager = new QNetworkConfigurationManager(this);
//    _networkInfo = new QSystemNetworkInfo(this);

    _determineCurrentWlanName();
    monitorConfiguration(true);        
}

ConditionManagerWlan::~ConditionManagerWlan() {
    monitorConfiguration(false);
}

void
ConditionManagerWlan::_setCurrentWlanName(const QNetworkConfiguration &nc, bool requestRefreshIfChanged) {
    QString previousWlanName = _currentWlanName;

    if (nc.bearerType() == QNetworkConfiguration::BearerWLAN &&
        nc.state() == QNetworkConfiguration::Active)
    {
        _currentWlanName = nc.name();
        qDebug("Selected %s as current Wlan name", qPrintable(_currentWlanName));
    } else if (nc.state() == QNetworkConfiguration::Active) {
        qDebug("This active, deselecting currentWlanName '%s'", qPrintable(_currentWlanName));
        _currentWlanName.clear();
    }

    qDebug("ConditionManagerWlan::_setCurrentWlanName: requestRefreshIfNeeded %d, previousWlanName %s, _currentWlanName %s",
           requestRefreshIfChanged, qPrintable(previousWlanName), qPrintable(_currentWlanName));

    if (requestRefreshIfChanged
            && previousWlanName != _currentWlanName
            && !_currentRuleWlanNames.contains(_currentRuleWlanNames)
            && (_watchedWlanNames.contains(previousWlanName)
                || _watchedWlanNames.contains(_currentWlanName))) {
        qDebug("ConditionManagerWlan::_setCurrentWlanName: requesting refresh");
        emit refreshNeeded();
    }
}

void
ConditionManagerWlan::_determineCurrentWlanName() {
    QList<QNetworkConfiguration> confs = _networkConfigurationManager->allConfigurations();
    QList<QNetworkConfiguration>::const_iterator i = confs.constBegin();
    qDebug("All Network configurations");
    for (; i != confs.constEnd(); ++i) {
        const QNetworkConfiguration &nc = *i;
        _logConfiguration(nc);
        _setCurrentWlanName(nc, false);
    }
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
ConditionManagerWlan::startRefresh() {
    _watchedWlanNames.clear();
}

bool
ConditionManagerWlan::refresh(const Rule &rule) {
    const QSet<QString> &wlanNames = rule.getWlan();
    if (wlanNames.isEmpty()) {
        qDebug("ConditionManagerWlan::refresh wlanNames is empty, matches");
        return true;
    }

    qDebug("ConditionManagerWlan::refresh rule %s, currentWlanName '%s'",
           qPrintable(rule.getRuleName()), qPrintable(_currentWlanName));

    _watchedWlanNames.unite(wlanNames);

    if (wlanNames.contains(_currentWlanName)) {
        qDebug("ConditionManagerWlan::refresh contains currentWlanName");
        return true;
    }

    return false;
}

void
ConditionManagerWlan::matchedRule(const Rule &rule) {
    _currentRuleWlanNames = rule.getWlan();
}

void
ConditionManagerWlan::endRefresh() {
    if (!_watchedWlanNames.isEmpty()) {
        _requestRefreshEnabled = true;
    } else {
        _requestRefreshEnabled = false;
        _currentRuleWlanNames.clear();
    }
}

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
//        connect(_networkConfigurationManager,
//                SIGNAL(configurationAdded(QNetworkConfiguration)),
//                this, SLOT(onConfigurationAdded(QNetworkConfiguration)));
//        connect(_networkConfigurationManager,
//                SIGNAL(configurationChanged(QNetworkConfiguration)),
//                this, SLOT(onConfigurationChanged(QNetworkConfiguration)));
//        connect(_networkConfigurationManager,
//                SIGNAL(configurationRemoved(QNetworkConfiguration)),
//                this, SLOT(onConfigurationRemoved(QNetworkConfiguration)));

//        connect(_networkInfo, SIGNAL(networkNameChanged(QSystemNetworkInfo::NetworkMode,QString)),
//                this, SLOT(onNetworkNameChanged(QSystemNetworkInfo::NetworkMode,QString)));
    }
}

void
ConditionManagerWlan::onConfigurationAdded(const QNetworkConfiguration   &config) {
    qDebug("Network configuration added");
    _logConfiguration(config);
    _setCurrentWlanName(config, _requestRefreshEnabled);
}

void
ConditionManagerWlan::onConfigurationChanged(const QNetworkConfiguration &config) {
    qDebug("Network configuration changed");
    _logConfiguration(config);
    _setCurrentWlanName(config, _requestRefreshEnabled);
}

void
ConditionManagerWlan::onConfigurationRemoved(const QNetworkConfiguration &config) {
    qDebug("Network configuration removed");
    _logConfiguration(config);
    _setCurrentWlanName(config, _requestRefreshEnabled);
}
