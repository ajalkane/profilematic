#include "conditionmanagerinternetconnectionmode.h"

ConditionManagerInternetConnectionMode::ConditionManagerInternetConnectionMode()
    : _currentInternetConnectionMode(RuleCondition::UndefinedInternetConnectionMode), _existsRulesWithInternetConnectionMode(false), _monitor(false)
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
ConditionManagerInternetConnectionMode::refresh(const RuleCondition &condition) {
    if (condition.getInternetConnectionMode() == RuleCondition::UndefinedInternetConnectionMode) {
        return true;
    }

    _existsRulesWithInternetConnectionMode = true;

    if (_currentInternetConnectionMode == RuleCondition::UndefinedInternetConnectionMode) {
        _currentInternetConnectionMode = mapNetworkMode(_networkConfigurationManager.defaultConfiguration().bearerType());
    }

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
    }
}

void
ConditionManagerInternetConnectionMode::_monitorNetworkMode(bool monitor) {
    qDebug("ConditionManagerInternetConnectionMode::monitorNetworkMode(%d), currently %d", monitor, _monitor);
    if (monitor != _monitor) {
        _monitor = monitor;

        if (monitor) {
            qDebug("ConditionManagerInternetConnectionMode::monitor network configuration change");
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
    qDebug("QNetworkConfiguration::bearerTypeName %s", qPrintable(config.bearerTypeName()));
    qDebug("QNetworkConfiguration::identifier     %s", qPrintable(config.identifier()));
    qDebug("QNetworkConfiguration::name           %s", qPrintable(config.name()));
    qDebug("QNetworkConfiguration::state          %0x", (int)config.state());
    qDebug(" ");
}

void
ConditionManagerInternetConnectionMode::onConfigurationChanged(const QNetworkConfiguration &config) {
    qDebug("%s Network configuration changed", qPrintable(QDateTime::currentDateTime().toString()));
    _logConfiguration(config);

    if (config.state() == QNetworkConfiguration::Active)
    {
        RuleCondition::InternetConnectionMode activeMode = mapNetworkMode(config.bearerType());
        if (activeMode != _currentInternetConnectionMode) {
            _currentInternetConnectionMode = activeMode;
            qDebug("ConditionManagerInternetConnectionMode::onConfigurationChanged requesting refresh");
            emit refreshNeeded();
        }
    }

}
