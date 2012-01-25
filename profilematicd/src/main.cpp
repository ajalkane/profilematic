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
#include <QProcess>

#include "profileclient.h"
#include "configuration.h"
#include "preferences.h"
#include "model/rule.h"
//#include "logic/rulewatch.h"
//#include "logic/ruleactivator.h"
#include "logic/rulesmanager.h"
#include "logic/conditionmanagerchain.h"
#include "logic/conditionmanagertime.h"
#include "logic/conditionmanagerlocationcell.h"
#include "logic/conditionmanagerwifi.h"
#include "logic/actionchain.h"
#include "logic/actionflightmode.h"
#include "logic/actionprofile.h"
#include "interface/profilematicinterface.h"
#include "platform/platformutil.h"

#include <stdio.h>

//#include <qmdevicemode.h>
#define CONVERSION_WARNING_CMDLINE "/opt/profilematic/bin/profilematic -conversionWarning"

ConditionManager *
buildConditionManager() {
    ConditionManagerChain *cm = new ConditionManagerChain();
    cm->add(new ConditionManagerTime());
    cm->add(new ConditionManagerLocationCell());
    cm->add(new ConditionManagerWifi());
    return cm;
}

Action *
buildAction(ProfileClient *profileClient, PlatformUtil *platformUtil) {
    ActionChain *ac = new ActionChain();
    ac->add(new ActionProfile(profileClient));
    ac->add(new ActionFlightMode(platformUtil));
    return ac;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ProfileClient profileClient;
    Preferences preferences;
    QScopedPointer<PlatformUtil> platformUtil(PlatformUtil::create());
    int rules_version = -1;
    QList<Rule> rules;
    Configuration::readRules(rules, &rules_version);
    Configuration::readPreferences(preferences);
    QScopedPointer<ConditionManager> conditionManager(buildConditionManager());
    QScopedPointer<Action> action(buildAction(&profileClient, platformUtil.data()));
    RulesManager rulesManager(&rules, conditionManager.data(), action.data(), &preferences);
    ProfileMaticInterface interface(&rulesManager, &rules, &preferences);

    if (interface.init()) {
        fprintf(stderr, "Exiting\n");
        return -1;
    }
    rulesManager.refresh();

    // LATER: this code can be removed a couple of versions down the road I think.
    qDebug("rules_version: %d", rules_version);
    if (rules_version == 0 && !rules.isEmpty()) {
        qDebug("Launching conversion warning");
        QProcess::startDetached(CONVERSION_WARNING_CMDLINE);
        qDebug("Conversion warning launched");
    }

    qDebug("Starting");
    int ret = a.exec();
    qDebug("Exiting\n");
    return ret;
}
