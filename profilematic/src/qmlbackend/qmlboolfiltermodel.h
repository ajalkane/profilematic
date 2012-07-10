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
#ifndef QMLBOOLFILTERMODEL_H
#define QMLBOOLFILTERMODEL_H

#include <QSortFilterProxyModel>

class QmlBoolFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    int  _filterOnRole;
    bool _filterCondition;

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

public:
    QmlBoolFilterModel(QAbstractItemModel *sourceModel, int filterOnRole, bool filterCondition, QObject *parent = 0);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private slots:
    void emitSizeChanged(const QModelIndex &, int, int);
signals:
    void sizeChanged();
};

#endif // QMLBOOLFILTERMODEL_H
