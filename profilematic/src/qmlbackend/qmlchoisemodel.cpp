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
#include <QVariantMap>

#include "qmlchoisemodel.h"

QmlChoiseModel::ChoiseItem::ChoiseItem()
    : itemId(-1)
{
}

QmlChoiseModel::ChoiseItem::ChoiseItem(const ChoiseItem &o) {
    *this = o;
}

QmlChoiseModel::QmlChoiseModel(QObject *parent)
: QAbstractListModel(parent)
{
    _roleToProperty[IdRole]                     = "mode";
    // Must be name for Harmattan dialog
    _roleToProperty[ShortDescriptionChoiseRole] = "name";
    _roleToProperty[LongDescriptionChoiseRole]  = "description";
    setRoleNames(_roleToProperty);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
}

void
QmlChoiseModel::emitSizeChanged(const QModelIndex &, int, int) {
    emit sizeChanged();
}

int
QmlChoiseModel::rowCount(const QModelIndex &) const {
    qDebug() << Q_FUNC_INFO << "size" << _choiseItems.size();
     return _choiseItems.size();
}

QVariant
QmlChoiseModel::data(const QModelIndex & index, int role) const {
    qDebug() << Q_FUNC_INFO << "role " << role;
    int row = index.row();
    if (row < 0 || row > _choiseItems.size() - 1) {
        qWarning() << Q_FUNC_INFO << "Invalid row " << row << "max" << _choiseItems.size();
        return QVariant();
    }

    const ChoiseItem &item = _choiseItems.at(row);
    switch (role) {
    case IdRole:
        return item.itemId;
    case ShortDescriptionChoiseRole:
        return item.shortDescription;
    case LongDescriptionChoiseRole:
        return item.longDescription;
    }

    return QVariant();
}

void
QmlChoiseModel::add(int itemId, const QString &shortDesc, const QString &longDesc) {
    qWarning() << Q_FUNC_INFO << "add" << itemId << shortDesc << longDesc;

    ChoiseItem item;
    item.itemId = itemId;
    item.shortDescription = shortDesc;
    item.longDescription = longDesc;

    beginInsertRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
    _choiseItems << item;
    endInsertRows();
}

QVariantMap
QmlChoiseModel::get(int row) {
    qDebug() << Q_FUNC_INFO << "get" << row;

    if (row < 0 || row > _choiseItems.size() - 1) {
        qWarning() << Q_FUNC_INFO << "Invalid row " << row << "max" << _choiseItems.size();
        return QVariantMap();
    }
    QVariantMap map;
    const ChoiseItem &item = _choiseItems.at(row);

    map[_roleToProperty[IdRole]] = item.itemId;
    map[_roleToProperty[ShortDescriptionChoiseRole]] = item.shortDescription;
    map[_roleToProperty[LongDescriptionChoiseRole]] = item.longDescription;

    return map;
}
