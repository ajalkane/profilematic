#include "conditionmanagernetworkmode.h"

ConditionManagerNetworkMode::ConditionManagerNetworkMode()
    : _currentNetworkMode(RuleCondition::UndefinedNetworkMode), _existsRulesWithNetworkMode(false), _monitor(false)
{
}

void
ConditionManagerNetworkMode::startRefresh() {
    _existsRulesWithNetworkMode = false;
}

RuleCondition::NetworkMode mapNetworkMode(QSystemNetworkInfo::NetworkMode mode) {
    if (mode == QSystemNetworkInfo::GsmMode)
        return RuleCondition::Gsm;
    if (mode == QSystemNetworkInfo::WlanMode)
        return RuleCondition::Wlan;

    qWarning("Warning: unknown network mode %d", mode);
    return RuleCondition::UndefinedNetworkMode;
}

bool
ConditionManagerNetworkMode::refresh(const RuleCondition &condition) {
    if (condition.getNetworkMode() == RuleCondition::UndefinedNetworkMode) {
        return true;
    }

    _existsRulesWithNetworkMode = true;

    if (_currentNetworkMode == RuleCondition::UndefinedNetworkMode) {
        _currentNetworkMode = mapNetworkMode(_networkInfo.currentMode());
    }

    return _currentNetworkMode == condition.getNetworkMode();
}

void
ConditionManagerNetworkMode::matchedRule(const RuleCondition &rule) {
    // NOP.
}

void
ConditionManagerNetworkMode::endRefresh() {
    _monitorNetworkMode(_existsRulesWithNetworkMode);
    if (!_existsRulesWithNetworkMode) {
        _currentNetworkMode = RuleCondition::UndefinedNetworkMode;
    }
}

void
ConditionManagerNetworkMode::networkModeChanged(QSystemNetworkInfo::NetworkMode mode) {
    qDebug("%s ConditionManagerNetworkMode:networkModeChanged(%d), refreshing", qPrintable(QDateTime::currentDateTime().toString()), mode);
    _currentNetworkMode = mapNetworkMode(mode);

    emit refreshNeeded();
}

void
ConditionManagerNetworkMode::_monitorNetworkMode(bool monitor) {
    qDebug("ConditionManagerNetworkMode::monitorNetworkMode(%d), currently %d", monitor, _monitor);
    if (monitor != _monitor) {
        if (monitor) {
            connect(&_networkInfo, SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)), this, SLOT(networkModeChanged(QSystemNetworkInfo::NetworkMode)), Qt::UniqueConnection);
        } else {
            disconnect(&_networkInfo, SIGNAL(networkModeChanged(QSystemNetworkInfo::NetworkMode)), this, SLOT(networkModeChanged(QSystemNetworkInfo::NetworkMode)));
        }
    }
}
