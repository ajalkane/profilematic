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
#include <QtCore/QCoreApplication>
#include <QDBusMetaType>

#include "profileclient.h"
#include "configuration.h"
#include "preferences.h"
#include "model/rule.h"
#include "logic/rulewatch.h"
#include "logic/ruleactivator.h"
#include "interface/profilematicinterface.h"
#include "platform/platformutil.h"

#include <stdio.h>

//#include <qmdevicemode.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

  //  MeeGo::QmDeviceMode deviceMode;

//    qDebug("DeviceMode: %d", deviceMode.getMode());

    ProfileClient profileClient;
    Preferences preferences;
    PlatformUtil *platformUtil = PlatformUtil::create();

    QList<Rule> rules;

    Configuration::readRules(rules);
    Configuration::readPreferences(preferences);
    RuleWatch ruleWatch(&rules, &preferences);
    RuleActivator ruleActivator(&profileClient, platformUtil);
    ProfileMaticInterface interface(&ruleWatch, &rules, &preferences);

    if (interface.init()) {
        fprintf(stderr, "Exiting\n");
        return -1;
    }
    ruleActivator.connect(&ruleWatch, SIGNAL(activateRule(Rule)), &ruleActivator, SLOT(activateRule(Rule)));

    ruleWatch.refreshWatch();

    qDebug("Starting");
    printf("Starting\n");
    int ret = a.exec();
    printf("Exiting\n");
    delete platformUtil;
    return ret;
}
