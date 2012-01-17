#include "conditionmanagerlocationcell.h"

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
    _currentCellId = -1;
}

bool
ConditionManagerLocationCell::refresh(const Rule &rule) {
    const QSet<int> &cellIds = rule.getLocationCells();
    if (cellIds.isEmpty()) {
        qDebug("ConditionManagerLocationCell::refresh cellIds is empty, matches");
        return true;
    }

    if (_currentCellId == -1) {
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
ConditionManagerLocationCell::endRefresh() {
    if (!_watchedCellIds.isEmpty()) {
        monitorCellId(true);
    } else {
        monitorCellId(false);
    }
}

void
ConditionManagerLocationCell::cellIdChanged(int cellId) {
    qDebug("ConditionManagerLocationCell::cellIdChanged to %d", cellId);
    if (_watchedCellIds.contains(cellId)) {
        qDebug("ConditionManagerLocationCell::cellIdChanged watched contains, requesting refresh");
        emit refreshNeeded();
    }
}

void
ConditionManagerLocationCell::monitorCellId(bool monitor) {
    qDebug("ConditionManagerLocationCell::monitorCellId(%d)", monitor);
    if (monitor) {
        connect(&_networkInfo, SIGNAL(cellIdChanged(int)), this, SIGNAL(cellIdChanged(int)), Qt::UniqueConnection);
    } else {
        disconnect(&_networkInfo, SIGNAL(cellIdChanged(int)), this, SIGNAL(cellIdChanged(int)));
    }
}
