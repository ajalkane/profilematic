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
#include <QDate>

#include "qmldaysmodel.h"

#define DAYS_IN_WEEK 7

QmlDaysModel::QmlDaysModel(QObject *parent)
    : QAbstractListModel(parent)
{
    _roleToProperty[DayIdRole]     = "dayId";
    _roleToProperty[NameRole]      = "name";
    _roleToProperty[ShortNameRole]  = "shortName";
    setRoleNames(_roleToProperty);
}

int
QmlDaysModel::rowCount(const QModelIndex &) const {
     return DAYS_IN_WEEK;
 }

QVariant
QmlDaysModel::data(const QModelIndex & index, int role) const {
    int row = index.row();
    if (row < 0 || row > DAYS_IN_WEEK - 1)
        return QVariant();

    switch (role) {
    case DayIdRole:
        return row + 1;
    case NameRole:
        return QDate::longDayName(row + 1, QDate::StandaloneFormat);
    case ShortNameRole:
        return QDate::shortDayName(row + 1, QDate::StandaloneFormat);
    }

    return QVariant();
}

QVariant
QmlDaysModel::getName(int index) const {
    QModelIndex modelIndex = createIndex(index, 0);
    return data(modelIndex, NameRole);
}

QVariant
QmlDaysModel::getShortName(int index) const {
    QModelIndex modelIndex = createIndex(index, 0);
    return data(modelIndex, ShortNameRole);
}
