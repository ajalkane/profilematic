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
#ifndef RULEACTIVATOR_H
#define RULEACTIVATOR_H

#include <QObject>

#include "rulewatch.h"
#include "../profileclient.h"
#include "../platform/platformutil.h"

class RuleActivator : public QObject
{
    Q_OBJECT

    ProfileClient *_profileClient;
    PlatformUtil *_platformUtil;
public:
    RuleActivator(ProfileClient *profileClient,
                  PlatformUtil *platformUtil);

public slots:
    void activateRule(const Rule &rule);
};

#endif // RULEACTIVATOR_H
