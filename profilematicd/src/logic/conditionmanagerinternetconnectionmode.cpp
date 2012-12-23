/**********************************************************************
 * Copyright 2012 Arto Jalkanen
 *
 * This file is part of ProfileMatic.
 *
 * ProfileMatic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProfileMatic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProfileMatic.  If not, see <http://www.gnu.org/licenses/>
**/

#include <QDebug>

#include "conditionmanagerinternetconnectionmode.h"

ConditionManagerInternetConnectionMode::ConditionManagerInternetConnectionMode()
    : _currentInternetConnectionMode(RuleCondition::UndefinedInternetConnectionMode),
      _existsRulesWithInternetConnectionMode(false)
{
    setObjectName("ConditionManagerInternetConnectionMode");
}

bool
ConditionManagerInternetConnectionMode::conditionSetForMatching(const RuleCondition &cond) const {
    return _conditionSetForMatching(cond);
}

RuleCondition::InternetConnectionMode
ConditionManagerInternetConnectionMode::_mapNetworkMode(const QNetworkConfiguration &conf) {
    if (conf == _networkConfigurationNone) {
        qWarning("ConditionManagerInternetConnectionMode::mapNetworkMode no internet connection, RuleCondition::None returned");
        return RuleCondition::ConnectionNone;
    }

    QNetworkConfiguration::BearerType bearerType = conf.bearerType();
    if (bearerType == QNetworkConfiguration::Bearer2G || bearerType == QNetworkConfiguration::BearerHSPA)
        return RuleCondition::Gsm;
    if (bearerType == QNetworkConfiguration::BearerWLAN)
        return RuleCondition::Wlan;

    qWarning() << "Warning: unrecognized bearer type" << bearerType;
    return RuleCondition::UndefinedInternetConnectionMode;
}

ConditionManagerCacheable::MatchStatus
ConditionManagerInternetConnectionMode::match(const Rule::IdType &, const RuleCondition &cond) {
    if (!_conditionSetForMatching(cond)) {
        qDebug() << "ConditionManagerInternetConnectionMode::match() options not set";
        return MatchNotSet;
    }

    _existsRulesWithInternetConnectionMode = true;

    if (_currentInternetConnectionMode == RuleCondition::UndefinedInternetConnectionMode) {
        QNetworkConfiguration conf = _getCurrentActiveConfiguration();
        _currentInternetConnectionMode = _mapNetworkMode(conf);
        _currentInternetConnectionIdentifier = conf.identifier();
        qDebug() << "ConditionManagerInternetConnectionMode::match() current mode not set,"
                 << "mapped to" <<  _currentInternetConnectionMode
                 << "id" << _currentInternetConnectionIdentifier;
    }

    if (cond.getInternetConnectionMode() == RuleCondition::ConnectionAny
            && _currentInternetConnectionMode != RuleCondition::ConnectionNone) {
        qDebug() << "ConditionManagerInternetConnectionMode::match() connectionAny and current connection != None (" << _currentInternetConnectionMode << ")";
        return Matched;
    }

    qDebug() << "ConditionManagerInternetConnectionMode::comparing"
             << _currentInternetConnectionMode
             << "=="
             << cond.getInternetConnectionMode();
    return _currentInternetConnectionMode == cond.getInternetConnectionMode() ? Matched : NotMatched;
}

void
ConditionManagerInternetConnectionMode::startMonitor() {
    qDebug() << "ConditionManagerInternetConnectionMode::startMonitor()";

    connect(&_networkConfigurationManager,
            SIGNAL(configurationChanged(const QNetworkConfiguration &)),
            this, SLOT(onConfigurationChanged(const QNetworkConfiguration &)));
}

void
ConditionManagerInternetConnectionMode::stopMonitor() {
    qDebug() << "ConditionManagerInternetConnectionMode::stopMonitor()";

    _currentInternetConnectionMode = RuleCondition::UndefinedInternetConnectionMode;
    _currentInternetConnectionIdentifier.clear();
    _existsRulesWithInternetConnectionMode = false;

    disconnect(&_networkConfigurationManager,
            SIGNAL(configurationChanged(const QNetworkConfiguration &)),
            this, SLOT(onConfigurationChanged(const QNetworkConfiguration &)));
}

void
ConditionManagerInternetConnectionMode::rulesChanged() {
    qDebug() << "ConditionManagerInternetConnectionMode::rulesChanged";

    _existsRulesWithInternetConnectionMode = false;
}

void _logConfiguration(const QNetworkConfiguration &config) {
    qDebug() << "ConditionManagerInternetConnectionMode::QNetworkConfiguration::bearerTypeName" << config.bearerTypeName();
    qDebug() << "ConditionManagerInternetConnectionMode::QNetworkConfiguration::identifier    " << config.identifier();
    qDebug() << "ConditionManagerInternetConnectionMode::QNetworkConfiguration::name          " << config.name();
    qDebug() << "ConditionManagerInternetConnectionMode::QNetworkConfiguration::state         " << (int)config.state();
    qDebug() << " ";
}

QNetworkConfiguration
ConditionManagerInternetConnectionMode::_getCurrentActiveConfiguration() const {
    qDebug() << "ConditionManagerInternetConnectionMode::_getCurrentActiveConfiguration";
    QList<QNetworkConfiguration> confs = _networkConfigurationManager.allConfigurations(QNetworkConfiguration::Active);
    foreach (const QNetworkConfiguration nc, confs) {
        _logConfiguration(nc);
        if (nc.state() == QNetworkConfiguration::Active) {
            return nc;
        }
    }

    qWarning() << "ConditionManagerInternetConnectionMode::_getCurrentActiveConfiguration: no active configuration found, returning None";
    return _networkConfigurationNone;
}

void
ConditionManagerInternetConnectionMode::onConfigurationChanged(const QNetworkConfiguration &config) {
    qDebug() << QDateTime::currentDateTime().toString() << "ConditionManagerInternetConnectionMode::Network configuration changed";
    _logConfiguration(config);

    if (config.state() == QNetworkConfiguration::Active) {
        RuleCondition::InternetConnectionMode activeMode = _mapNetworkMode(config);
        if (activeMode != _currentInternetConnectionMode) {
            _currentInternetConnectionMode = activeMode;
            _currentInternetConnectionIdentifier = config.identifier();
            _existsRulesWithInternetConnectionMode = false;
            qDebug() << "ConditionManagerInternetConnectionMode::onConfigurationChanged requesting refresh"
                     << "mode:" << _currentInternetConnectionMode
                     << "id:" << _currentInternetConnectionIdentifier;
            emit matchInvalidated();
        }
    }
    // Note: this could cause unnecessary refreshes if old connection becomes non-Active before
    // a new connection becomes active. At least on harmattan the new connection becomes
    // active first, so this is not an issue and works as intended. The intention is that
    // refresh will be requested if there is no internet connection at all.
    else if (_currentInternetConnectionIdentifier == config.identifier() && !_currentInternetConnectionIdentifier.isEmpty()) {
        qDebug() << "ConditionManagerInternetConnectionMode::onConfigurationChanged"
                 << "current connection" << _currentInternetConnectionIdentifier
                 << "not active anymore, invalidating";
        _currentInternetConnectionMode = RuleCondition::UndefinedInternetConnectionMode;
        _currentInternetConnectionIdentifier.clear();
        _existsRulesWithInternetConnectionMode = false;
        emit matchInvalidated();
    }
}
