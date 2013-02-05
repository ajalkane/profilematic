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

#include "qmlselectedapplicationsmodel.h"

QmlSelectedApplicationsModel::QmlSelectedApplicationsModel(Rule *rule, QmlApplicationsScanner *scanner, QObject *parent)
    : super(parent), _editRule(rule), _scanner(scanner)
{
    QHash<int, QByteArray> roleToProperty;

    roleToProperty[ApplicationLauncher] = "launcher";
    roleToProperty[ApplicationName] = "name";
    roleToProperty[IconUri] = "iconuri";

    setRoleNames(roleToProperty);

    connect(&(_editRule->action()), SIGNAL(applicationChanged()), this, SLOT(_launchersChanged()));
}

QVariant
QmlSelectedApplicationsModel::data(const QModelIndex &index, int role) const {
    // qDebug() <<  "QmlSelectedApplicationsModel::data for index" << index << "role" << role;
    if (index.row() < 0 || index.row() >= _launchers.count() || _editRule == 0)
        return QVariant();

    const QString &launcher = _launchers.at(index.row()); // (_editRule->action().application().getLaunchers())[index.row()];
    switch (role) {
    case ApplicationLauncher:
        return launcher;
    case ApplicationName:
        return _nameForLauncher(launcher);
    case IconUri:
        return _iconForLauncher(launcher);
    }
    return QVariant();
}

void
QmlSelectedApplicationsModel::initializeEdit() {
    _launchersChanged();
}

const QmlApplication &
QmlSelectedApplicationsModel::_appForLauncher(const QString &launcher) const {
    if (!_launcherToApp.contains(launcher)) {
        QmlApplication app = _scanner->applicationFromLauncher(launcher);
        _launcherToApp[launcher] = app;
    }
    return _launcherToApp[launcher];
}

const QString &
QmlSelectedApplicationsModel::_nameForLauncher(const QString &launcher) const {
    const QmlApplication &app = _appForLauncher(launcher);
    return app.applicationName();
}

const QString &
QmlSelectedApplicationsModel::_iconForLauncher(const QString &launcher) const {
    const QmlApplication &app = _appForLauncher(launcher);
    return app.iconUri();
}

void
QmlSelectedApplicationsModel::_launchersChanged() {
    qDebug() << "QmlSelectedApplicationsModel::launchersChanged";

    beginResetModel();

    _launcherToApp.clear();
    _launchers.clear();

    if (_editRule == 0) {
        qDebug() << "QmlSelectedApplicationsModel null edit rule";
    } else {
        foreach (const QString &launcher, _editRule->action().application().getLaunchers()) {
            qDebug() << "QmlSelectedApplicationsModel adding launcher " << launcher;
            _launchers << launcher;
        }
    }

    endResetModel();
    emit sizeChanged();
}
