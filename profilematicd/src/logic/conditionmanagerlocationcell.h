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
#include <QTimer>
#include <QSet>
#include <QSystemNetworkInfo>
#include <QSystemAlignedTimer>

QTM_USE_NAMESPACE

#include "conditionmanagercacheable.h"
#include "../platform/platformutil.h"
#include "../util/pmtimer.h"

class ConditionManagerLocationCell  : public ConditionManagerCacheable {
    Q_OBJECT

    QSystemNetworkInfo _networkInfo;

    QSet<int> _watchedCellIds;
    QSet<int> _currentRuleCellIds;
    int _currentTimeout;
    int _currentCellId;
    PmTimer _cellsTimeout;

    bool _enteredNonWatchedCell();

    void _clearVarsForInvalidating();
    inline bool _conditionSetForMatching(const RuleCondition &cond) const { return !cond.getLocationCells().isEmpty(); }

public:
    ConditionManagerLocationCell(QObject *parent = 0);
    virtual ~ConditionManagerLocationCell();

    virtual bool conditionSetForMatching(const RuleCondition &cond) const;
    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond);

    virtual void startMonitor();
    virtual void stopMonitor();

    virtual void rulesChanged();

public slots:
    void cellIdChanged(int cellId);
    void onCurrentCellsTimeout();
};

#endif // CONDITIONMANAGERLOCATIONCELL_H
