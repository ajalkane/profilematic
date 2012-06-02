#include "conditionmanagerinternetconnectionmode.h"

ConditionManagerInternetConnectionMode::ConditionManagerInternetConnectionMode()
    : _currentInternetConnectionMode(RuleCondition::UndefinedInternetConnectionMode), _existsRulesWithInternetConnectionMode(false), _monitor(false)
{
}

void
ConditionManagerInternetConnectionMode::startRefresh() {
    _existsRulesWithInternetConnectionMode = false;
}

RuleCondition::InternetConnectionMode mapNetworkMode(QSystemNetworkInfo::NetworkMode mode) {
    if (mode == QSystemNetworkInfo::GsmMode)
        return RuleCondition::Gsm;
    if (mode == QSystemNetworkInfo::WlanMode)
        return RuleCondition::Wlan;

    qWarning("Warning: unknown network mode %d", mode);
    return RuleCondition::UndefinedInternetConnectionMode;
}

bool
ConditionManagerInternetConnectionMode::refresh(const RuleCondition &condition) {
    if (condition.getInternetConnectionMode() == RuleCondition::UndefinedInternetConnectionMode) {
        return true;
    }

    _existsRulesWithInternetConnectionMode = true;

    if (_currentInternetConnectionMode == RuleCondition::UndefinedInternetConnectionMode) {
        _currentInternetConnectionMode = mapNetworkMode(_networkInfo.currentMode());
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
ConditionManagerInternetConnectionMode::mynetworkModeChanged(QSystemNetworkInfo::NetworkMode mode) {
    qDebug("%s ConditionManagerInternetConnectionMode:networkModeChanged(%d), refreshing", qPrintable(QDateTime::currentDateTime().toString()), mode);
    _currentInternetConnectionMode = mapNetworkMode(mode);

    emit refreshNeeded();
}

void
ConditionManagerInternetConnectionMode::_monitorNetworkMode(bool monitor) {
    qDebug("ConditionManagerInternetConnectionMode::monitorNetworkMode(%d), currently %d", monitor, _monitor);
    if (monitor != _monitor) {
        _monitor = monitor;
        if (monitor) {
            bool success = connect(&_networkInfo, SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)), this, SLOT(mynetworkModeChanged(QSystemNetworkInfo::NetworkMode)));
            qDebug("ConditionManagerInternetConnectionMode::monitorNetworkMode connected signal return value %d", success);
        } else {
            qDebug("ConditionManagerInternetConnectionMode::monitorNetworkMode disconnecting signal");
            disconnect(&_networkInfo, SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)), this, SLOT(mynetworkModeChanged(QSystemNetworkInfo::NetworkMode)));
        }
    }
}
