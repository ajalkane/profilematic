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
#ifndef CONDITIONMANAGERWIFI_H
#define CONDITIONMANAGERWIFI_H

#include <QObject>
#include <QSet>
#include <QNetworkConfigurationManager>

#include "conditionmanager.h"
#include "../platform/platformutil.h"

class ConditionManagerWifi : public ConditionManager {
    Q_OBJECT

    QNetworkConfigurationManager *_networkConfigurationManager;

    void _logConfiguration(const QNetworkConfiguration &config) const;

public:
    ConditionManagerWifi(QObject *parent = 0);
    virtual ~ConditionManagerWifi();

    virtual void startRefresh();
    virtual bool refresh(const Rule &rule);
    virtual void matchedRule(const Rule &rule);
    virtual void endRefresh();

    void monitorConfiguration(bool monitor);

public slots:
    void onConfigurationAdded(const QNetworkConfiguration   &config);
    void onConfigurationChanged(const QNetworkConfiguration &config);
    void onConfigurationRemoved(const QNetworkConfiguration &config);
};

#endif // CONDITIONMANAGERWIFI_H
