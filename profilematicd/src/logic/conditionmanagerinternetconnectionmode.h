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

#include "conditionmanagercacheable.h"

class ConditionManagerInternetConnectionMode : public ConditionManagerCacheable {
    Q_OBJECT

    RuleCondition::InternetConnectionMode _currentInternetConnectionMode;
    QString _currentInternetConnectionIdentifier;
    QNetworkConfigurationManager _networkConfigurationManager;
    QNetworkConfiguration _networkConfigurationNone;

    bool _existsRulesWithInternetConnectionMode;

    RuleCondition::InternetConnectionMode _mapNetworkMode(const QNetworkConfiguration &conf);
    QNetworkConfiguration _getCurrentActiveConfiguration() const;

    inline bool _conditionSetForMatching(const RuleCondition &cond) const {
        return cond.getInternetConnectionMode() != RuleCondition::UndefinedInternetConnectionMode;
    }

    void _clearVarsForInvalidation();
public:
    ConditionManagerInternetConnectionMode();

    virtual bool conditionSetForMatching(const RuleCondition &cond) const;
    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond);

    virtual void startMonitor();
    virtual void stopMonitor();

    virtual void rulesChanged();

private slots:
    void onConfigurationChanged(const QNetworkConfiguration &config);
};

#endif // CONDITIONMANAGERINTERNETCONNECTIONMODE_H
