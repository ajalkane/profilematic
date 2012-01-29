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
#ifndef CONDITIONMANAGERWLAN_H
#define CONDITIONMANAGERWLAN_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QNetworkConfigurationManager>
#include <QSystemAlignedTimer>

QTM_USE_NAMESPACE

#include "conditionmanager.h"
#include "../platform/platformutil.h"

class ConditionManagerWlan : public ConditionManager {
    Q_OBJECT

    QNetworkConfigurationManager *_networkConfigurationManager;

    QSet<QString> _watchedWlanNames;
    QSet<QString> _currentRuleWlanNames;
    QString _currentWlanName;
    QSystemAlignedTimer _wlanTimeout;
    int _currentRuleWlanTimeoutSecs;

    bool _monitoring;
    bool _requestRefreshEnabled;
    void _wlanNotActive();
    void _wlanActive(const QString &name);
    void _logConfiguration(const QNetworkConfiguration &config) const;
    void _determineCurrentWlanName(bool requestRefreshIfChanged);
    void _setCurrentWlanName(const QNetworkConfiguration &config, bool requestRefreshIfChanged);

public:
    ConditionManagerWlan(QObject *parent = 0);
    virtual ~ConditionManagerWlan();

    virtual void startRefresh();
    virtual bool refresh(const Rule &rule);
    virtual void matchedRule(const Rule &rule);
    virtual void endRefresh();

    void monitorConfiguration(bool monitor);

public slots:
    void onConfigurationAdded(const QNetworkConfiguration   &config);
    void onConfigurationChanged(const QNetworkConfiguration &config);
    void onConfigurationRemoved(const QNetworkConfiguration &config);
    void onWlanTimeout();
};

#endif // CONDITIONMANAGERWLAN_H
