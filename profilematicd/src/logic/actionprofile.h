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
#ifndef ACTIONPROFILE_H
#define ACTIONPROFILE_H

#include "actionstatefulbase.h"
#include "../profileclient.h"
#include "../model/rule.h"

class ActionProfile: public ActionStatefulBase
{
    ProfileClient *_profileClient;
    QString _previousProfile;
    int _previousVolume;
public:
    ActionProfile(ProfileClient *profileClient);

    virtual bool activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule);
};

#endif // ACTIONPROFILE_H
