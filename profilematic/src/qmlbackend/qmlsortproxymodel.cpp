/**********************************************************************
 * Copyright 2013 Arto Jalkanen
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
#include <QDebug>

#include "qmlsortproxymodel.h"

QmlSortProxyModel::QmlSortProxyModel(QAbstractItemModel *sourceModel, int sortOnRole, QObject *parent)
    : QSortFilterProxyModel(parent), _sortOnRole(sortOnRole)
{
    setSourceModel(sourceModel);
    setSortRole(sortOnRole);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setSortLocaleAware(true);

    setDynamicSortFilter(true);
    sort(0);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
}

void
QmlSortProxyModel::emitSizeChanged(const QModelIndex &, int, int) {
    qDebug() << "QmlSortProxyModel::emitSizeChanged";
    emit sizeChanged();
}

int
QmlSortProxyModel::mapToSourceIndex(int i) {
    QModelIndex mi = index(i, 0);
    qWarning("QmlSortProxyModel::mapToSourceIndex(%d)", i);

    if (!mi.isValid()) {
        qWarning("QmlSortProxyModel::mapToSourceIndex: Invalid index %d", i);
        return -1;
    }

    return mapToSource(mi).row();
}
