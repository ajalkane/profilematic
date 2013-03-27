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
#include <QtGlobal>
#include <QDebug>
#include "qmlprofilesmodel.h"

QmlProfilesModel::QmlProfilesModel(ProfileClient *profileClient, Role nameRole, QObject *parent)
    : QAbstractListModel(parent), _profileClient(profileClient)
{
    // Default Harmattan profile types for translators
    QT_TR_NOOP("Ringing");
    QT_TR_NOOP("Silent");
    QT_TR_NOOP("Beep");
    QT_TR_NOOP("Don't change");

    _roleToProperty[Profile] = "profile";
    _roleToProperty[ProfileType] = "profileType";
    _roleToProperty[nameRole]    = "name";
    setRoleNames(_roleToProperty);

    _init(nameRole);
}

void
QmlProfilesModel::_init(Role /*nameRole*/) {
    Q_ASSERT_X(nameRole == ProfileType, "QmlProfileModel", "Only ProfileType role supported at the moment");

    QString dontChangeProfileType = "Don't change";
    _nameToProfile[dontChangeProfileType] = "";
    _profileToName[""] = "";
    _names.append(dontChangeProfileType);

    QStringList profiles = _profileClient->getProfiles();
    QString profile;
    foreach (profile, profiles) {
        // In Harmattan an "outdoors" profile is returned which has
        // the same type as ringing. It can't be selected from the
        // user interface, so skip that one outright.
        if (profile != NULL && profile.toLower() != "outdoors") {
            QString profileType = _profileClient->getProfileType(profile);
            if (!_nameToProfile.contains(profileType)) {
                _nameToProfile[profileType] = profile;
                _profileToName[profile] = profileType;
                _names.append(profileType);
            }
        }
    }
}

int
QmlProfilesModel::rowCount(const QModelIndex &) const {
     return _names.size();
 }

QVariant
QmlProfilesModel::data(const QModelIndex & index, int role) const {
    int row = index.row();
    if (row < 0 || row > _names.size() - 1)
        return QVariant();

    const QString &name = _names.at(row);
    switch (role) {
    case Profile:
        return _nameToProfile[name];
    case ProfileType:
        return tr(name.toLatin1());
    }

    return QVariant();
}

QVariant
QmlProfilesModel::getProfile(int index) const {
    QModelIndex modelIndex = createIndex(index, 0);
    return data(modelIndex, Profile);
}

QVariant
QmlProfilesModel::getProfileType(int index) const {
    QModelIndex modelIndex = createIndex(index, 0);
    return data(modelIndex, ProfileType);
}

QString
QmlProfilesModel::getProfileToName(const QString &profile) const {
    // qDebug("QmlProfilesModel::getProfileToName(%s)", qPrintable(profile));
    if (_profileToName.contains(profile)) {
        // qDebug("QmlProfilesModel::getProfileToName returning from _profileToName");
        QString profileName = _profileToName[profile];

        return tr(profileName.toLatin1());
    }
    // qDebug("QmlProfilesModel::getProfileToName returning unrecognized");

    return tr("Unrecognized %1").arg(profile);
}

bool
QmlProfilesModel::profileHasVolume(const QString &profile) const {
    // qDebug("QmlProfilesModel::profileHasVolume(%s)", qPrintable(profile));
    // Hard-coded to be the Ringing profile type for now, maybe there could be a more dynamic way?
    // QString profileType = getProfileToName(profile);
    // qDebug("QmlProfilesModel::profileHasVolume: profileType %s", qPrintable(profileType));
    // return profileType.toLower() == "ringing";

    // Now like this, as it faciliates translations
    qDebug() << Q_FUNC_INFO << "profile" << profile;
    return profile.toLower() == "general";
}
