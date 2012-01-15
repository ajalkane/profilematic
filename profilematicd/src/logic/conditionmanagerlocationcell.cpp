#include "conditionmanagerlocationcell.h"

ConditionManagerLocationCell::ConditionManagerLocationCell(PlatformUtil *platformUtil, QObject *parent)
    : ConditionManager(parent), _platformUtil(platformUtil), _currentCellId(-1)
{
}

ConditionManagerLocationCell::~ConditionManagerLocationCell() {}

void
ConditionManagerLocationCell::startRefresh() {
    _watchedCellIds.clear();
    _currentCellId = -1;
}

bool
ConditionManagerLocationCell::refresh(const Rule &rule) {
    const QList<int> &cellIds = rule.getLocationCells();
    if (cellIds.isEmpty()) {
        qDebug("ConditionManagerLocationCell::refresh cellIds is empty, matches");
        return true;
    }

    if (_currentCellId == -1) {
        _currentCellId = _platformUtil->cellId();
    }
    qDebug("ConditionManagerLocationCell::refresh rule %s, currentCellId %d",
           qPrintable(rule.getRuleName()), _currentCellId);

    QList<int>::const_iterator cellIds_i = cellIds.constBegin();
    for (; cellIds_i != cellIds.constEnd(); ++cellIds_i) {
        _watchedCellIds << *cellIds_i;
    }

    if (cellIds.contains(_currentCellId)) {
        qDebug("ConditionManagerLocationCell::refresh contains currentCellId");
        return true;
    }

    return false;
}

void
ConditionManagerLocationCell::endRefresh() {
    if (!_watchedCellIds.isEmpty()) {
        connect(_platformUtil, SIGNAL(cellIdChanged(int)), this, SLOT(cellIdChanged(int)), Qt::UniqueConnection);
        _platformUtil->monitorCellId(true);
    } else {
        _platformUtil->monitorCellId(false);
        disconnect(_platformUtil, SIGNAL(cellIdChanged(int)), this, SLOT(cellIdChanged(int)));        
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
