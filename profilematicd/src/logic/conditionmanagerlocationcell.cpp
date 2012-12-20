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

ConditionManagerLocationCell::ConditionManagerLocationCell(QObject *parent)
    : ConditionManagerCacheable(parent), _currentTimeout(-1), _currentCellId(CELL_ID_NOT_QUERIED)
{
    setObjectName("ConditionManagerLocationCell");

    _cellsTimeout.setSingleShot(true);
    connect(&_cellsTimeout, SIGNAL(timeout()), this, SLOT(onCurrentCellsTimeout()));
}

ConditionManagerLocationCell::~ConditionManagerLocationCell() {
    stopMonitor();
}

bool
ConditionManagerLocationCell::conditionSetForMatching(const RuleCondition &cond) const {
    return _conditionSetForMatching(cond);
}

ConditionManagerCacheable::MatchStatus
ConditionManagerLocationCell::match(const Rule::IdType &, const RuleCondition &cond) {
    if (!_conditionSetForMatching(cond)) {
        qDebug() << "ConditionManagerLocationCell::match() options not set or invalid, matchNotSet";
        return MatchNotSet;
    }

    const QSet<int> &cellIds = cond.getLocationCells();

    if (_currentCellId == CELL_ID_NOT_QUERIED) {
        _currentCellId = _networkInfo.cellId();
    }
    qDebug() << "ConditionManagerLocationCell::match() currentCellId" << _currentCellId;

    _watchedCellIds.unite(cellIds);

    if (cellIds.contains(_currentCellId)) {
        qDebug() << "ConditionManagerLocationCell::match() contains currentCellId";
        _currentRuleCellIds.unite(cellIds);
        _currentTimeout = qMax(_currentTimeout, cond.getLocationCellsTimeout());
        return Matched;
    }

    return NotMatched;
}

//void
//ConditionManagerLocationCell::startRefresh() {
//    _currentRuleCellIds.clear();
//    _watchedCellIds.clear();
//    _currentTimeout = -1;
//}

void
ConditionManagerLocationCell::onCurrentCellsTimeout() {
    qDebug() << QDateTime::currentDateTime().toString() << "ConditionManagerLocationCell:onCurrentCellsRefreshNeeded, invalidating";
    _clearVars();
    emit matchInvalidated();
}

bool
ConditionManagerLocationCell::_enteredNonWatchedCell() {
    if (_currentTimeout > 0) {
        if (!_cellsTimeout.isActive()) {
            qDebug() << QDateTime::currentDateTime().toString()
                     << "ConditionManagerLocationCell::_enteredNonWatchedCell() timeout specified,"
                     << "waiting" << _currentTimeout << "seconds before invalidating";
            _cellsTimeout.start(_currentTimeout, _currentTimeout + 1);
        }
        return false;
    } else {
        return true;
    }
}

void
ConditionManagerLocationCell::cellIdChanged(int cellId) {
    qDebug() << "ConditionManagerLocationCell::cellIdChanged to" << cellId;
    // 0 comes when no mobile network connection
    if (cellId <= 0) {
        qDebug() << "ConditionManagerLocationCell::cellIdChanged ignoring <= 0 cellId";
        return;
    }

    bool invalidate = false;

    if (_currentRuleCellIds.contains(cellId)) {
        qDebug() << "ConditionManagerLocationCell::cellIdChanged current rule has this cellId";
        if (_cellsTimeout.isActive()) _cellsTimeout.stop();
    }
    else if (_watchedCellIds.contains(cellId)) {
        qDebug() << "ConditionManagerLocationCell::cellIdChanged watched contains and is not in current Rule's cellIds, invalidating";
        if (_cellsTimeout.isActive()) _cellsTimeout.stop();
        invalidate = true;
    }
    else if (!_currentRuleCellIds.isEmpty()) {
        if (!_enteredNonWatchedCell()) {
            qDebug() << "ConditionManagerLocationCell::cellIdChanged, not anymore in current rule's timeout active";
            // If timeout is used, current cell id must not be set
            return;
        } else {
            qDebug() << "ConditionManagerLocationCell::cellIdChanged, not anymore in current rule's cells, invalidating";
            invalidate = true;
        }
    }
    else {
        qDebug() << "ConditionManagerLocationCell::cellIdChanged but not in active cellIds, no need to invalidate";
    }
    _currentCellId = cellId;
    if (invalidate) {
        qDebug() << "ConditionManagerLocationCell::cellIdChanged matchInvalidated";
        _clearVars();
        emit matchInvalidated();
    }
}

void
ConditionManagerLocationCell::startMonitor() {
    qDebug() << "ConditionManagerLocationCell::startMonitor";
    connect(&_networkInfo, SIGNAL(cellIdChanged(int)), this, SLOT(cellIdChanged(int)));
}

void
ConditionManagerLocationCell::stopMonitor() {
    qDebug() << "ConditionManagerLocationCell::stopMonitor";
    disconnect(&_networkInfo, SIGNAL(cellIdChanged(int)), this, SLOT(cellIdChanged(int)));

    _clearVars();
}

void
ConditionManagerLocationCell::rulesChanged() {
    qDebug() << "ConditionManagerLocationCell::rulesChanged";

    _clearVars();
}

void
ConditionManagerLocationCell::_clearVars() {
    _currentRuleCellIds.clear();
    _watchedCellIds.clear();
    _currentTimeout = -1;
    _currentCellId = CELL_ID_NOT_QUERIED;
    _cellsTimeout.stop();
}
