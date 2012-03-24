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
#ifndef CONDITIONMANAGERLOCATIONCELL_H
#define CONDITIONMANAGERLOCATIONCELL_H

#include <QObject>
#include <QSet>
#include <QSystemNetworkInfo>

QTM_USE_NAMESPACE

#include "conditionmanager.h"
#include "../platform/platformutil.h"

class ConditionManagerLocationCell : public ConditionManager {
    Q_OBJECT

    QSystemNetworkInfo _networkInfo;

    QSet<int> _watchedCellIds;
    QSet<int> _currentRuleCellIds;
    int _currentCellId;

public:
    ConditionManagerLocationCell(QObject *parent = 0);
    virtual ~ConditionManagerLocationCell();

    virtual void startRefresh();
    virtual bool refresh(const RuleCondition &rule);
    virtual void matchedRule(const RuleCondition &rule);
    virtual void endRefresh();

    void monitorCellId(bool monitor);

public slots:
    void cellIdChanged(int cellId);
};

#endif // CONDITIONMANAGERLOCATIONCELL_H
