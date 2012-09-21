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

// Use -2 as the unset current cell id, since -1 is used by the framework for
// not being able to get cell id.
#define CELL_ID_NOT_QUERIED -2

// TODO: this class need UnitTest
ConditionManagerLocationCell::ConditionManagerLocationCell(QObject *parent)
    : ConditionManager(parent), _currentTimeout(-1), _currentCellId(CELL_ID_NOT_QUERIED)
{
    _cellsTimeout.setSingleShot(true);
    connect(&_cellsTimeout, SIGNAL(timeout()), this, SLOT(onCurrentCellsTimeout()));
}

ConditionManagerLocationCell::~ConditionManagerLocationCell() {
    monitorCellId(false);
}

void
ConditionManagerLocationCell::startRefresh() {
    _currentRuleCellIds.clear();
    _watchedCellIds.clear();
    _currentTimeout = -1;
}

bool
ConditionManagerLocationCell::refresh(const Rule::IdType &, const RuleCondition &rule) {
    const QSet<int> &cellIds = rule.getLocationCells();
    if (cellIds.isEmpty()) {
        qDebug("ConditionManagerLocationCell::refresh cellIds is empty, matches");
        return true;
    }

    if (_currentCellId == CELL_ID_NOT_QUERIED) {
        _currentCellId = _networkInfo.cellId();
    }
    qDebug("ConditionManagerLocationCell::refresh currentCellId %d", _currentCellId);

    _watchedCellIds.unite(cellIds);

    if (cellIds.contains(_currentCellId)) {
        qDebug("ConditionManagerLocationCell::refresh contains currentCellId");
        return true;
    }

    return false;
}

void
ConditionManagerLocationCell::matchedRule(const RuleCondition &rule) {
    _currentRuleCellIds.unite(rule.getLocationCells());
    _currentTimeout = qMax(_currentTimeout, rule.getLocationCellsTimeout());
}

void
ConditionManagerLocationCell::endRefresh() {    
    if (!_watchedCellIds.isEmpty()) {
        monitorCellId(true);
    } else {
        monitorCellId(false);
        _currentRuleCellIds.clear();
        _currentCellId = CELL_ID_NOT_QUERIED;
        if (_cellsTimeout.isActive()) _cellsTimeout.stop();
    }
}

void
ConditionManagerLocationCell::onCurrentCellsTimeout() {
    qDebug("%s ConditionManagerLocationCell:onCurrentCellsRefreshNeeded, refreshing", qPrintable(QDateTime::currentDateTime().toString()));
    _currentCellId = CELL_ID_NOT_QUERIED;
    emit refreshNeeded();
}

bool
ConditionManagerLocationCell::_enteredNonWatchedCell() {
    if (_currentTimeout > 0) {
        if (!_cellsTimeout.isActive()) {
            qDebug("%s ConditionManagerLocationCell::_enteredNonWatchedCell() timeout specified, waiting %d seconds before refresh",
                   qPrintable(QDateTime::currentDateTime().toString()), _currentTimeout);
            _cellsTimeout.start(_currentTimeout, _currentTimeout + 1);
        }
        return false;
    } else {
        return true;
    }
}

void
ConditionManagerLocationCell::cellIdChanged(int cellId) {
    qDebug("ConditionManagerLocationCell::cellIdChanged to %d", cellId);
    // 0 comes when no mobile network connection
    if (cellId <= 0) {
        qDebug("ConditionManagerLocationCell::cellIdChanged ignoring <= 0 cellId");
        return;
    }

    bool refresh = false;

    if (_currentRuleCellIds.contains(cellId)) {
        qDebug("ConditionManagerLocationCell::cellIdChanged current rule has this cellId");
        if (_cellsTimeout.isActive()) _cellsTimeout.stop();
    }
    else if (_watchedCellIds.contains(cellId)) {
        qDebug("ConditionManagerLocationCell::cellIdChanged watched contains and is not in current Rule's cellIds, requesting refresh");
        if (_cellsTimeout.isActive()) _cellsTimeout.stop();
        refresh = true;
    }
    else if (!_currentRuleCellIds.isEmpty()) {
        if (!_enteredNonWatchedCell()) {
            qDebug("ConditionManagerLocationCell::cellIdChanged, not anymore in current rule's timeout active");
            // If timeout is used, current cell id must not be set
            return;
        } else {
            qDebug("ConditionManagerLocationCell::cellIdChanged, not anymore in current rule's cells, requesting refresh");
            refresh = true;
        }
    }
    else {
        qDebug("ConditionManagerLocationCell::cellIdChanged but not in active cellIds, no refresh");
    }
    _currentCellId = cellId;
    if (refresh) emit refreshNeeded();
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
