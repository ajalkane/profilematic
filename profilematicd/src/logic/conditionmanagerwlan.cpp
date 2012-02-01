#include "conditionmanagerwlan.h"

namespace {
    QString WLAN_NAME_NEEDS_REFRESH("|DrFrsh|");
}

ConditionManagerWlan::ConditionManagerWlan(QObject *parent)
    : ConditionManager(parent),
      _currentWlanName(WLAN_NAME_NEEDS_REFRESH),
      _currentRuleWlanTimeoutSecs(0),
      _monitoring(false)
{
    _networkConfigurationManager = new QNetworkConfigurationManager(this);
    _wlanTimeout.setSingleShot(true);
    connect(&_wlanTimeout, SIGNAL(timeout()), this, SLOT(onWlanTimeout()));
}

ConditionManagerWlan::~ConditionManagerWlan() {
    monitorConfiguration(false);
}

void
ConditionManagerWlan::_wlanNotActive() {
    qDebug("ConditionManagerWlan: _wlanNotActive, timeout %d secs", _currentRuleWlanTimeoutSecs);
    if (_currentRuleWlanTimeoutSecs <= 0) {
        _currentWlanName.clear();
    } else if (_currentRuleWlanNames.contains(_currentWlanName)) {
        qDebug("ConditionManagerWlan: starting WLAN timeout timer");
        _wlanTimeout.start(_currentRuleWlanTimeoutSecs, _currentRuleWlanTimeoutSecs + 1);
    }
}

void
ConditionManagerWlan::_wlanActive(const QString &name) {
    qDebug("%s ConditionManagerWlan::_wlanActive %s", qPrintable(QDateTime::currentDateTime().toString()), qPrintable(name));
    _currentWlanName = name;
    _wlanTimeout.stop();
}

void
ConditionManagerWlan::onWlanTimeout() {
    qDebug("%s ConditionManagerWlan: WLAN timeout reached, refreshing", qPrintable(QDateTime::currentDateTime().toString()));
    _currentWlanName.clear();
    emit refreshNeeded();
}

void
ConditionManagerWlan::_setCurrentWlanName(const QNetworkConfiguration &nc, bool requestRefreshIfChanged) {
    QString previousWlanName = _currentWlanName;

    if (nc.bearerType() == QNetworkConfiguration::BearerWLAN &&
        nc.state() == QNetworkConfiguration::Active)
    {
        _wlanActive(nc.name());
        qDebug("Selected %s as current Wlan name", qPrintable(_currentWlanName));
    } else if (nc.state() == QNetworkConfiguration::Active) {
        qDebug("Active %s, currentWlanName '%s'", qPrintable(nc.name()), qPrintable(_currentWlanName));
    } else if (nc.state() != QNetworkConfiguration::Active && nc.name() == _currentWlanName) {
        qDebug("Was current WlanName, but not active anymore, clearing '%s'", qPrintable(_currentWlanName));
        _wlanNotActive();
    }

    qDebug("ConditionManagerWlan::_setCurrentWlanName: requestRefreshIfNeeded %d, previousWlanName %s, _currentWlanName %s",
           requestRefreshIfChanged, qPrintable(previousWlanName), qPrintable(_currentWlanName));

    if (requestRefreshIfChanged
            && previousWlanName != _currentWlanName
            && !_currentRuleWlanNames.contains(_currentWlanName)
            && (_watchedWlanNames.contains(previousWlanName)
                || _watchedWlanNames.contains(_currentWlanName))) {
        qDebug("ConditionManagerWlan::_setCurrentWlanName: requesting refresh");
        emit refreshNeeded();
    }
}

void
ConditionManagerWlan::_determineCurrentWlanName(bool requestRefreshIfChanged) {
    // On Harmattan, platform updates the configuration on the background so no need to request updates manually
    QList<QNetworkConfiguration> confs = _networkConfigurationManager->allConfigurations(QNetworkConfiguration::Active);
    QList<QNetworkConfiguration>::const_iterator i = confs.constBegin();
    qDebug("Active Network configurations");
    for (; i != confs.constEnd(); ++i) {
        const QNetworkConfiguration &nc = *i;
        _logConfiguration(nc);
        _setCurrentWlanName(nc, requestRefreshIfChanged);
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

    if (_currentWlanName == WLAN_NAME_NEEDS_REFRESH) {
        _determineCurrentWlanName(false);
    }

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
    _currentRuleWlanTimeoutSecs = rule.getWlanTimeout();
}

void
ConditionManagerWlan::endRefresh() {
    if (!_watchedWlanNames.isEmpty()) {
        monitorConfiguration(true);
    } else {
        qDebug("ConditionManagerWlan::endRefresh, stopping monitoring");
        monitorConfiguration(false);
        _currentRuleWlanNames.clear();
        // If not monitoring the configuration, we can't be certain the current Wlan name is known
        _currentWlanName = WLAN_NAME_NEEDS_REFRESH;
        _wlanTimeout.stop();
    }
}

void
ConditionManagerWlan::monitorConfiguration(bool monitor) {
    if (monitor && !_monitoring) {
        connect(_networkConfigurationManager,
                SIGNAL(configurationAdded(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationAdded(const QNetworkConfiguration &)));
        connect(_networkConfigurationManager,
                SIGNAL(configurationChanged(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationChanged(const QNetworkConfiguration &)));
        connect(_networkConfigurationManager,
                SIGNAL(configurationRemoved(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationRemoved(const QNetworkConfiguration &)));
    } else if (!monitor && _monitoring) {
        disconnect(_networkConfigurationManager,
                SIGNAL(configurationAdded(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationAdded(const QNetworkConfiguration &)));
        disconnect(_networkConfigurationManager,
                SIGNAL(configurationChanged(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationChanged(const QNetworkConfiguration &)));
        disconnect(_networkConfigurationManager,
                SIGNAL(configurationRemoved(const QNetworkConfiguration &)),
                this, SLOT(onConfigurationRemoved(const QNetworkConfiguration &)));
    }

    _monitoring = monitor;
}

void
ConditionManagerWlan::onConfigurationAdded(const QNetworkConfiguration   &config) {
    qDebug("%s Network configuration added", qPrintable(QDateTime::currentDateTime().toString()));
    _logConfiguration(config);
    _setCurrentWlanName(config, true);
}

void
ConditionManagerWlan::onConfigurationChanged(const QNetworkConfiguration &config) {
    qDebug("%s Network configuration changed", qPrintable(QDateTime::currentDateTime().toString()));
    _logConfiguration(config);
    _setCurrentWlanName(config, true);
}

void
ConditionManagerWlan::onConfigurationRemoved(const QNetworkConfiguration &config) {
    qDebug("%s Network configuration removed", qPrintable(QDateTime::currentDateTime().toString()));
    _logConfiguration(config);
    if (config.name() == _currentWlanName) {
        _currentWlanName = WLAN_NAME_NEEDS_REFRESH;
        _determineCurrentWlanName(true);
    }
}
