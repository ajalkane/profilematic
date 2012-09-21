#include "conditionmanagerinternetconnectionmode.h"

ConditionManagerInternetConnectionMode::ConditionManagerInternetConnectionMode()
    : _currentInternetConnectionMode(RuleCondition::UndefinedInternetConnectionMode),
      _existsRulesWithInternetConnectionMode(false),
      _monitor(false)
{
}

void
ConditionManagerInternetConnectionMode::startRefresh() {
    _existsRulesWithInternetConnectionMode = false;
}

RuleCondition::InternetConnectionMode mapNetworkMode(QNetworkConfiguration::BearerType type) {
    if (type == QNetworkConfiguration::Bearer2G || type == QNetworkConfiguration::BearerHSPA)
        return RuleCondition::Gsm;
    if (type == QNetworkConfiguration::BearerWLAN)
        return RuleCondition::Wlan;

    qWarning("Warning: unrecognized bearer type %d", type);
    return RuleCondition::UndefinedInternetConnectionMode;
}

bool
ConditionManagerInternetConnectionMode::refresh(const Rule::IdType &, const RuleCondition &condition) {
    if (condition.getInternetConnectionMode() == RuleCondition::UndefinedInternetConnectionMode) {
        qDebug("ConditionManagerInternetConnectionMode::refresh not set, matches");
        return true;
    }

    _existsRulesWithInternetConnectionMode = true;

    if (_currentInternetConnectionMode == RuleCondition::UndefinedInternetConnectionMode) {
        QNetworkConfiguration conf = _getCurrentActiveConfiguration();
        _currentInternetConnectionMode = mapNetworkMode(conf.bearerType());
        _currentInternetConnectionIdentifier = conf.identifier();
        qDebug("ConditionManagerInternetConnectionMode::refresh current mode not set, mapped to %d (%s)",
               _currentInternetConnectionMode, qPrintable(_currentInternetConnectionIdentifier));
    }

    qDebug("ConditionManagerInternetConnectionMode::comparing %d == %d", _currentInternetConnectionMode, condition.getInternetConnectionMode());
    return _currentInternetConnectionMode == condition.getInternetConnectionMode();
}

void
ConditionManagerInternetConnectionMode::matchedRule(const RuleCondition &) {
    // NOP.
}

void
ConditionManagerInternetConnectionMode::endRefresh() {
    _monitorNetworkMode(_existsRulesWithInternetConnectionMode);
    if (!_existsRulesWithInternetConnectionMode) {
        _currentInternetConnectionMode = RuleCondition::UndefinedInternetConnectionMode;
        _currentInternetConnectionIdentifier.clear();
    }
}

void
ConditionManagerInternetConnectionMode::_monitorNetworkMode(bool monitor) {
    qDebug("ConditionManagerInternetConnectionMode::monitorNetworkMode(%d), currently %d", monitor, _monitor);
    if (monitor != _monitor) {
        _monitor = monitor;

        if (monitor) {
            qDebug("ConditionManagerInternetConnectionMode::monitor network configuration changes");
            connect(&_networkConfigurationManager,
                    SIGNAL(configurationChanged(const QNetworkConfiguration &)),
                    this, SLOT(onConfigurationChanged(const QNetworkConfiguration &)));
        } else {
            qDebug("ConditionManagerInternetConnectionMode::monitor disconnect network configuration change signal");
            disconnect(&_networkConfigurationManager,
                    SIGNAL(configurationChanged(const QNetworkConfiguration &)),
                    this, SLOT(onConfigurationChanged(const QNetworkConfiguration &)));
        }
    }
}

void _logConfiguration(const QNetworkConfiguration &config) {
    qDebug("ConditionManagerInternetConnectionMode::QNetworkConfiguration::bearerTypeName %s", qPrintable(config.bearerTypeName()));
    qDebug("ConditionManagerInternetConnectionMode::QNetworkConfiguration::identifier     %s", qPrintable(config.identifier()));
    qDebug("ConditionManagerInternetConnectionMode::QNetworkConfiguration::name           %s", qPrintable(config.name()));
    qDebug("ConditionManagerInternetConnectionMode::QNetworkConfiguration::state          %0x", (int)config.state());
    qDebug(" ");
}

QNetworkConfiguration
ConditionManagerInternetConnectionMode::_getCurrentActiveConfiguration() const {
    qWarning("ConditionManagerInternetConnectionMode::_getCurrentActiveConfiguration");
    QList<QNetworkConfiguration> confs = _networkConfigurationManager.allConfigurations(QNetworkConfiguration::Active);
    foreach (const QNetworkConfiguration nc, confs) {
        _logConfiguration(nc);
        if (nc.state() == QNetworkConfiguration::Active) {
            return nc;
        }
    }

    qWarning("ConditionManagerInternetConnectionMode::_getCurrentActiveConfiguration: no active configuration found, returning default");
    return _networkConfigurationManager.defaultConfiguration();
}

void
ConditionManagerInternetConnectionMode::onConfigurationChanged(const QNetworkConfiguration &config) {
    qDebug("%s Network configuration changed", qPrintable(QDateTime::currentDateTime().toString()));
    _logConfiguration(config);

    if (config.state() == QNetworkConfiguration::Active) {
        RuleCondition::InternetConnectionMode activeMode = mapNetworkMode(config.bearerType());
        if (activeMode != _currentInternetConnectionMode) {
            _currentInternetConnectionMode = activeMode;
            _currentInternetConnectionIdentifier = config.identifier();
            qDebug("ConditionManagerInternetConnectionMode::onConfigurationChanged requesting refresh (%d/%s)",
                   _currentInternetConnectionMode, qPrintable(_currentInternetConnectionIdentifier));
            emit refreshNeeded();
        }
    }
    // Note: this could cause unnecessary refreshes if old connection becomes non-Active before
    // a new connection becomes active. At least on harmattan the new connection becomes
    // active first, so this is not an issue and works as intended. The intention is that
    // refresh will be requested if there is no internet connection at all.
    else if (_currentInternetConnectionIdentifier == config.identifier() && !_currentInternetConnectionIdentifier.isEmpty()) {
        qDebug("ConditionManagerInternetConnectionMode::onConfigurationChanged current connection (%s) not active anymore, refresh",
               qPrintable(_currentInternetConnectionIdentifier));
        _currentInternetConnectionMode = RuleCondition::UndefinedInternetConnectionMode;
        _currentInternetConnectionIdentifier.clear();
        emit refreshNeeded();
    }
}
