/**********************************************************************
 * Copyright 2011 Arto Jalkanen
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
#ifndef QMLDAYSMODEL_H
#define QMLDAYSMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

class QmlDaysModel : public QAbstractListModel
{
    Q_OBJECT

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount CONSTANT)

    QHash<int, QByteArray> _roleToProperty;
public:
    enum DayRoles {
        DayIdRole = Qt::UserRole + 1,
        NameRole,
        ShortNameRole
    };

    QmlDaysModel(QObject *parent = 0);

    // QAbstractListModel implementations
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QVariant getName(int index) const;
    Q_INVOKABLE QVariant getShortName(int index) const;

};

#endif // QMLDAYSMODEL_H
