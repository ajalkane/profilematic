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
#include "conditionmanagerlocationcell.h"

// TODO: this class need UnitTest
ConditionManagerLocationCell::ConditionManagerLocationCell(QObject *parent)
    : ConditionManager(parent), _currentCellId(-1)
{
}

ConditionManagerLocationCell::~ConditionManagerLocationCell() {
    monitorCellId(false);
}

void
ConditionManagerLocationCell::startRefresh() {
    _watchedCellIds.clear();
    // Use -2 as the unset current cell id, since -1 is used by the framework for
    // not being able to get cell id.
    _currentCellId = -2;
}

bool
ConditionManagerLocationCell::refresh(const Rule &rule) {
    const QSet<int> &cellIds = rule.getLocationCells();
    if (cellIds.isEmpty()) {
        qDebug("ConditionManagerLocationCell::refresh cellIds is empty, matches");
        return true;
    }

    if (_currentCellId == -2) {
        _currentCellId = _networkInfo.cellId();
    }
    qDebug("ConditionManagerLocationCell::refresh rule %s, currentCellId %d",
           qPrintable(rule.getRuleName()), _currentCellId);

    _watchedCellIds.unite(cellIds);

    if (cellIds.contains(_currentCellId)) {
        qDebug("ConditionManagerLocationCell::refresh contains currentCellId");
        return true;
    }

    return false;
}

void
ConditionManagerLocationCell::matchedRule(const Rule &rule) {
    _currentRuleCellIds = rule.getLocationCells();
}

void
ConditionManagerLocationCell::endRefresh() {    
    if (!_watchedCellIds.isEmpty()) {
        monitorCellId(true);
    } else {
        monitorCellId(false);
        _currentRuleCellIds.clear();
    }
}

void
ConditionManagerLocationCell::cellIdChanged(int cellId) {
    qDebug("ConditionManagerLocationCell::cellIdChanged to %d", cellId);
    // 0 comes when no mobile network connection
    if (cellId <= 0) {
        qDebug("ConditionManagerLocationCell::cellIdChanged ignoring <= 0 cellId");
    }
    else if (_currentRuleCellIds.contains(cellId)) {
        qDebug("ConditionManagerLocationCell::cellIdChanged current rule has this cellId");
    }
    else if (_watchedCellIds.contains(cellId)) {
        qDebug("ConditionManagerLocationCell::cellIdChanged watched contains and is not in current Rule's cellIds, requesting refresh");
        emit refreshNeeded();
    }
    else if (!_currentRuleCellIds.isEmpty()) {
        qDebug("ConditionManagerLocationCell::cellIdChanged, not anymore in current rule's cells, requesting refresh");
        emit refreshNeeded();
    }
    else {
        qDebug("ConditionManagerLocationCell::cellIdChanged but not in active cellIds, no refresh");
    }
}

void
ConditionManagerLocationCell::monitorCellId(bool monitor) {
    qDebug("ConditionManagerLocationCell::monitorCellId(%d)", monitor);
    if (monitor) {
        connect(&_networkInfo, SIGNAL(cellIdChanged(int)), this, SLOT(cellIdChanged(int)), Qt::UniqueConnection);
    } else {
        disconnect(&_networkInfo, SIGNAL(cellIdChanged(int)), this, SLOT(cellIdChanged(int)));
    }
}
