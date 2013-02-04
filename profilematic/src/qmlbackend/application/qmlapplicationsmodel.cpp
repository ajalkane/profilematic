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
#include "qmlapplicationsmodel.h"

QmlApplicationsModel::QmlApplicationsModel(QObject *parent)
    : super(parent)
{
    QHash<int, QByteArray> roleToProperty;

    roleToProperty[ApplicationLauncher] = "launcher";
    roleToProperty[ApplicationName]     = "name";
    roleToProperty[IconUri]             = "iconuri";

    setRoleNames(roleToProperty);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
}

void
QmlApplicationsModel::emitSizeChanged(const QModelIndex &, int, int) {
    emit sizeChanged();
}

QVariant
QmlApplicationsModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= _apps.count())
        return QVariant();

    const QmlApplication &app = _apps.at(index.row());
    switch (role) {
    case ApplicationLauncher:
        return app.applicationLauncher();
    case ApplicationName:
        return app.applicationName();
    case IconUri:
        return app.iconUri();
    }
    return QVariant();
}

void
QmlApplicationsModel::addApplication(const QmlApplication &app) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _apps << app;
    endInsertRows();
}

void
QmlApplicationsModel::clearApplications() {
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    _apps.clear();
    endRemoveRows();
}

