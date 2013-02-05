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
#ifndef QMLSORTPROXYMODEL_H
#define QMLSORTPROXYMODEL_H

#include <QSortFilterProxyModel>

class QmlSortProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    int  _sortOnRole;

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

public:
    QmlSortProxyModel(QAbstractItemModel *sourceModel, int sortOnRole, QObject *parent = 0);

    Q_INVOKABLE int mapToSourceIndex(int index);

private slots:
    void emitSizeChanged(const QModelIndex &, int, int);
signals:
    void sizeChanged();
};

#endif // QMLSORTPROXYMODEL_H
