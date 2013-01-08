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
#include <QtCore/QStringList>
#include <QtDBus/QtDBus>

#include "profileclient.h"
#include "util/conditionallogging.h"

#define PROFILED_SERVICE "com.nokia.profiled"
#define PROFILED_PATH "/com/nokia/profiled"
#define PROFILED_INTERFACE "com.nokia.profiled"
#define PROFILED_GET_PROFILES "get_profiles"
#define PROFILED_GET_PROFILE "get_profile"
#define PROFILED_SET_PROFILE "set_profile"
#define PROFILED_GET_VALUE "get_value"
#define PROFILED_SET_VALUE "set_value"
#define PROFILED_PROFILE_CHANGED "profile_changed"
// The key for accessing Harmattan's profile type under profile
#define PROFILED_TYPE_VALUE "ringing.alert.type"
#define PROFILED_VOLUME_VALUE "ringing.alert.volume"

ProfileClient::ProfileClient(QObject *parent) : QObject(parent)
{
//    if (!QDBusConnection::sessionBus().connect("", "",
//                                               PROFILED_INTERFACE, PROFILED_PROFILE_CHANGED,
//                                               this, SLOT(profileChanged(bool, bool, QString))))
//    {
//        qDebug("Failed to connect dbus signal isValid: %d", QDBusConnection::sessionBus().lastError().isValid());
//        qDebug("Failed to connect dbus signal type: %d", QDBusConnection::sessionBus().lastError().type());
//        qDebug("Failed to connect dbus signal: %s", qPrintable(QDBusConnection::sessionBus().lastError().name()));
//    }
}

ProfileClient::~ProfileClient()
{
//    if (!QDBusConnection::sessionBus().disconnect(PROFILED_SERVICE, PROFILED_PATH,
//                                               PROFILED_INTERFACE, PROFILED_PROFILE_CHANGED, this, SLOT(profileChanged(bool, bool, QString))))
//    {
//        qDebug("Failed to disconnect dbus signal: %s", qPrintable(QDBusConnection::sessionBus().lastError().message()));
//    }
}

QStringList
ProfileClient::getProfiles() const
{
    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
                              PROFILED_INTERFACE);

    QDBusReply<QStringList> reply = dbus_iface.call(PROFILED_GET_PROFILES);
    QStringList profiles = reply.value();
    // For testing on Desktop without profiled running
#ifndef __arm__
    profiles.append("Profile 1");
    profiles.append("Profile 2");
    profiles.append("Profile 3");
    profiles.append("general");
#endif

    return profiles;
}

QString
ProfileClient::getProfile() const {
    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
                              PROFILED_INTERFACE);

    QDBusReply<QString> reply = dbus_iface.call(PROFILED_GET_PROFILE);

    if (reply.isValid()) {
        return reply.value();
    } else {
        qWarning("ProfileClient::getProfile returned invalid reply");
    }

    return QString();
}

bool
ProfileClient::setProfile(const QString &profileName)
{
    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
                              PROFILED_INTERFACE);

    IFDEBUG(qDebug("Setting profile to %s", profileName.toLatin1().constData()));
    QDBusReply<bool> reply = dbus_iface.call(PROFILED_SET_PROFILE, profileName);
    if (reply.isValid()) {
        bool replyValue = reply.value();
        IFDEBUG(qDebug("Return value %d", replyValue == true));
        return replyValue;
    } else {
        IFDEBUG(qDebug("ProfileClient::getProfileType returned invalid reply for profile %s", qPrintable(profileName)));
    }
    return false;
}

int
ProfileClient::getProfileVolume(const QString &profile) const {
    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
                              PROFILED_INTERFACE);

    QDBusReply<QString> reply = dbus_iface.call(PROFILED_GET_VALUE, profile, PROFILED_VOLUME_VALUE);

    if (reply.isValid()) {
        QString value = reply.value();
        bool isOk = false;
        int volume = value.toInt(&isOk);
        if (isOk) {
            return volume;
        } else {
            IFDEBUG(qDebug("ProfileClient::getProfileVolume returned non-int for profile %s: %s", qPrintable(profile), qPrintable(value)));
        }
    } else {
        IFDEBUG(qDebug("ProfileClient::getProfileVolume returned invalid reply for profile %s", qPrintable(profile)));
    }

    return -1;
}

bool
ProfileClient::setProfileVolume(const QString &profileName, int volume)
{
    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
                              PROFILED_INTERFACE);

    IFDEBUG(qDebug("Setting profile %s to volume %d", profileName.toLatin1().constData(), volume));
    QDBusReply<bool> reply = dbus_iface.call(PROFILED_SET_VALUE, profileName,
                                             PROFILED_VOLUME_VALUE, QString::number(volume));
    if (reply.isValid()) {
        bool replyValue = reply.value();
        IFDEBUG(qDebug("Return value %d", replyValue == true));
        return replyValue;
    } else {
        IFDEBUG(qDebug("ProfileClient::setProfileVolume returned invalid reply for profile %s", qPrintable(profileName)));
    }
    return false;
}

QString
ProfileClient::getProfileType(const QString &profile) const {
    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
                              PROFILED_INTERFACE);

    QDBusReply<QString> reply = dbus_iface.call(PROFILED_GET_VALUE, profile, PROFILED_TYPE_VALUE);

    if (reply.isValid()) {
        return reply.value();
    } else {
        IFDEBUG(qDebug("ProfileClient::getProfileType returned invalid reply for profile %s", qPrintable(profile)));
    }

#ifndef __arm__
    return profile == "general" ? "Ringing" : profile + " Type";
#endif

    return QString();
}

//QStringList
//ProfileClient::profilesWithType() const
//{
//    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
//                              PROFILED_INTERFACE);

//    QStringList profilesWithType;
//    QString profile;
//    foreach (profile, profiles())
//    {
//        QDBusReply<QString> reply = dbus_iface.call(PROFILED_GET_VALUE, profile, PROFILED_TYPE_VALUE);
//        QString profileWithType = profile;
//        profileWithType.append(" (");
//        profileWithType.append(reply.value());
//        profileWithType.append(")");
//        profilesWithType.append(profileWithType);
//    }

//    return profilesWithType;
//}

//QStringList
//ProfileClient::profileTypes() const
//{
//    QDBusInterface dbus_iface(PROFILED_SERVICE, PROFILED_PATH,
//                              PROFILED_INTERFACE);

//    QStringList profileTypes;
//    QString profile;
//    foreach (profile, profiles())
//    {
//        QDBusReply<QString> reply = dbus_iface.call(PROFILED_GET_VALUE, profile, PROFILED_TYPE_VALUE);
//        if (reply.isValid()) {
//            profileTypes.append(reply.value());
//        }
//    }

//    // In Harmattan at least, profile type Ringing is attached to
//    // "general" and "outdoors" profiles. The "general" profile is
//    // the one that's used for "ringing", profile "outdoors" should
//    // not be used when setting a profile.
//    profileTypes.removeDuplicates();

//    qDebug("Add");
//    // Test data for desktop
//#ifndef __arm__
//    profileTypes.append("Profile 1");
//    profileTypes.append("Profile 2");
//    profileTypes.append("Profile 3");
//#endif

//    return profileTypes;
//}

//void
//ProfileClient::profileChanged(bool changed, bool active, QString profile)
//{
//    if (changed && active) {
//        qDebug("Profile changed %d/%d/%s", changed, active, qPrintable(profile));
//    }
//}

