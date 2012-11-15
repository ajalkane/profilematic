/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
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
#ifndef CONDITIONMANAGERINTERNETCONNECTIONMODE_H
#define CONDITIONMANAGERINTERNETCONNECTIONMODE_H

#include <QNetworkConfigurationManager>

#include "conditionmanager.h"

class ConditionManagerInternetConnectionMode : public ConditionManager {
    Q_OBJECT

    RuleCondition::InternetConnectionMode _currentInternetConnectionMode;
    QString _currentInternetConnectionIdentifier;
    QNetworkConfigurationManager _networkConfigurationManager;
    QNetworkConfiguration _networkConfigurationNone;

    bool _existsRulesWithInternetConnectionMode;
    bool _monitor;

    RuleCondition::InternetConnectionMode _mapNetworkMode(const QNetworkConfiguration &conf);
    QNetworkConfiguration _getCurrentActiveConfiguration() const;
    void _monitorNetworkMode(bool monitor);
public:
    ConditionManagerInternetConnectionMode();

    virtual void startRefresh();
    virtual bool refresh(const Rule::IdType &, const RuleCondition &rule);
    virtual void matchedRule(const RuleCondition &rule);
    virtual void endRefresh();

private slots:
    void onConfigurationChanged(const QNetworkConfiguration &config);
};

#endif // CONDITIONMANAGERINTERNETCONNECTIONMODE_H
