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
#include "ruleactivator.h"

RuleActivator::RuleActivator(ProfileClient *profileClient)
    : _profileClient(profileClient)
{    
}

void
RuleActivator::activateRule(const RuleItem &ruleItem) {
    QString profile = ruleItem.rule.getProfile();
    int profileVolume = ruleItem.rule.getProfileVolume();

    qDebug("RuleActivator::activateRule for rule %s, profile %s, volume %d",
           qPrintable(ruleItem.rule.getRuleName()),
           qPrintable(profile),
           profileVolume);

    _profileClient->setProfile(profile);
    // IMPROVE: hard-coded for now only profile "general" to have volume.
    // Should do something better. Also there's duplication, qmlprofilesmodel
    // also has same kind of code to determine if volume available. Maybe
    // move to profileclient.cpp
    if (profile.toLower() == "general" && profileVolume > -1 && profileVolume <= 100) {
        _profileClient->setProfileVolume(profile, profileVolume);
    }
}

