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
#include "qmlboolfiltermodel.h"

QmlBoolFilterModel::QmlBoolFilterModel(QAbstractItemModel *sourceModel, int filterOnRole, bool filterCondition, bool pdebugLog, QObject *parent)
    : QSortFilterProxyModel(parent), _filterOnRole(filterOnRole), _filterCondition(filterCondition), _debugLog(pdebugLog)
{
    setSourceModel(sourceModel);
    setDynamicSortFilter(true);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));

    debugLog("Constructor");
}

void
QmlBoolFilterModel::debugLog() {
    debugLog("unspecified");
}

void
QmlBoolFilterModel::debugLog(const QString &where) {
    if (!_debugLog) return;

    qDebug("QmlBoolFilterModel::debugLog() %s", qPrintable(where));
    qDebug("source rowCount %d, filter rowCount %d", sourceModel()->rowCount(), rowCount());
    for (int i = 0; i < sourceModel()->rowCount(); ++i) {
        QModelIndex sIndex = sourceModel()->index(i, 0);
        QModelIndex fIndex = mapFromSource(sIndex);
        qDebug("source %i -> filter %i", sIndex.row(), fIndex.row());
    }
    for (int i = 0; i < rowCount(); ++i) {
        QModelIndex fIndex = index(i, 0);
        QModelIndex sIndex = mapToSource(fIndex);
        qDebug("filter %i -> source %i", fIndex.row(), sIndex.row());
    }
}

void
QmlBoolFilterModel::emitSizeChanged(const QModelIndex &, int, int) {
    // qDebug("QmlBoolFilterModel::emitSizeChanged");
    emit sizeChanged();
    debugLog("emitSizeChanged");

}

bool
QmlBoolFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    // qDebug("QmlBoolFilterModel::filterAcceptsRow called");
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QVariant value = sourceModel()->data(index, _filterOnRole);

    return value.toBool() != _filterCondition;
}

int
QmlBoolFilterModel::mapToSourceIndex(int i) {
    debugLog("mapToSourceIndex");

    QModelIndex mi = index(i, 0);
    qWarning("QmlBoolFilterModel::mapToSourceIndex(%d)", i);

    if (!mi.isValid()) {
        qWarning("QmlBoolFilterModel::mapToSourceIndex: Invalid index %d", i);
        return -1;
    }

    return mapToSource(mi).row();
}
