/**********************************************************************
 * Copyright 2012 Arto Jalkanen
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
#include "actionprofile.h"

ActionProfile::ActionProfile(ProfileClient *profileClient)
    : _profileClient(profileClient), _previousVolume(-1)
{
}

void
ActionProfile::activate(const RuleAction &rule) {
    QString profile = rule.getProfile();
    int profileVolume = rule.getProfileVolume();
    bool profileIsEmpty = profile.trimmed().isEmpty();

    // TODO isDefaultRule
    if ((profileIsEmpty /*|| rule.isDefaultRule()*/) && !_previousProfile.isEmpty()) {
        qDebug("ActionProfile::activate restore, profile not set or is default rule");
        qDebug("ActionProfile::activate previous rule had restore profile, restoring profile %s",
               qPrintable(_previousProfile));
        profile = _previousProfile;
        profileVolume = _previousVolume;
        _previousProfile = "";
        _previousVolume = -1;
    }
    else if (profileIsEmpty) {
        qDebug("ActionProfile::activate profile is empty, not setting");
        return;
    }
    if (rule.getRestoreProfile()) {
        _previousProfile = _profileClient->getProfile();
        _previousVolume = _profileClient->getProfileVolume(_previousProfile);
    } else {
        _previousProfile = "";
        _previousVolume = -1;
    }

    qDebug("ActionProfile::activateRule profile %s, volume %d (previous %s/%d)",
           qPrintable(profile),
           profileVolume,
           qPrintable(_previousProfile),
           _previousVolume);

    _profileClient->setProfile(profile);
    // IMPROVE: hard-coded for now only profile "general" to have volume.
    // Should do something better. Also there's duplication, qmlprofilesmodel
    // also has same kind of code to determine if volume available. Maybe
    // move to profileclient.cpp
    if (profile.toLower() == "general" && profileVolume > -1 && profileVolume <= 100) {
        _profileClient->setProfileVolume(profile, profileVolume);
    }
}
