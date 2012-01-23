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
#ifndef PROFILECLIENT_H
#define PROFILECLIENT_H

#include <QtCore/QStringList>

#include <QtCore/QObject>

class ProfileClient : public QObject
{
    Q_OBJECT

public:
    ProfileClient(QObject *parent = 0);
    virtual ~ProfileClient();

    QStringList getProfiles() const;

    QString getProfile() const;
    bool setProfile(const QString &profileName);

    int getProfileVolume(const QString &profileName) const;
    bool setProfileVolume(const QString &profileName, int volume);

    QString getProfileType(const QString &profile) const;
    // QStringList profilesWithType() const;

    // Shows only the profile type. This is used in Harmattan.
    //QStringList profileTypes() const;


public slots:
    // void profileChanged(bool changed, bool active, QString profile);

//    void setProfileType(QString type);

//signals:
};

#endif // PROFILECLIENT_H
