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
#include <QDebug>
#include <QtCore/QCoreApplication>
#include <QDBusMetaType>
#include <QProcess>

#include "model/rule.h"

#include "profileclient.h"
#include "configuration.h"
#include "preferences.h"
#include "logic/rulesholder.h"
#include "logic/rulesmanager.h"
#include "logic/conditionmanagerfactory.h"
#include "logic/actionfactory.h"
#include "interface/profilematicinterface.h"
#include "platform/platformutil.h"
#include "util/conditionallogging.h"

#include <stdio.h>

#define CONVERSION_WARNING_CMDLINE "/opt/profilematic/bin/profilematic -conversionWarning"
#define CREDENTIAL_WARNING_CMDLINE "/opt/profilematic/bin/profilematic -credentialWarning"
#define MULTIRULE_WARNING_CMDLINE "/opt/profilematic/bin/profilematic -multiRuleWarning"

void noLoggingSink(QtMsgType, const char *) {
    // NOP
}

void credentialsCheck(const RulesHolder &rulesHolder) {
    bool hasRulesThatNeedDeviceModeCredential = false;
    foreach (const RuleHolder &ruleHolder, rulesHolder.ruleHolders()) {
        const Rule &rule = ruleHolder.rule();
        if (rule.action().getFlightMode() >= 0 /* ||
            Seems like power saving mode works even without the credential?
            rule.action().getPowerSavingMode() >= 0 */) {
            hasRulesThatNeedDeviceModeCredential = true;
        }
    }

    if (hasRulesThatNeedDeviceModeCredential && !PlatformUtil::instance()->hasDeviceModeCredential()) {
        IFDEBUG(qDebug("Launching credential warning"));
        QProcess::startDetached(CREDENTIAL_WARNING_CMDLINE);
        IFDEBUG(qDebug("Credential warning launched"));
    }
}

int main(int argc, char *argv[])
{    
    QCoreApplication a(argc, argv);    

    if (argc >= 2 && QLatin1String("-noDebug") == argv[1]) {
        qDebug("Inhibiting logging");
        ConditionalLogging::debugEnabled(false);
        // qInstallMsgHandler(noLoggingSink);
    }

    PlatformUtil::initialize();
    ProfileClient profileClient;
    Preferences preferences;
    int rules_version = -1;
    // TODO
    RulesHolder rulesHolder(&profileClient);
    Configuration::readRules(rulesHolder, &rules_version);
    Configuration::readPreferences(preferences);
    RulesManager rulesManager(&rulesHolder, &preferences);
    ProfileMaticInterface interface(&rulesManager, &rulesHolder, &preferences, &profileClient);

    if (interface.init()) {
        fprintf(stderr, "Exiting\n");
        PlatformUtil::deinitialize();
        return -1;
    }

    credentialsCheck(rulesHolder);

    IFDEBUG(qDebug("main: refresh"));

    rulesManager.refresh();

    // LATER: this code can be removed a couple of versions down the road I think.
    IFDEBUG(qDebug("rules_version: %d", rules_version));
    // rules.size > 1 because default rule always exists
    if (rulesHolder.size() > 1) {
        switch (rules_version) {
        case 0:
            IFDEBUG(qDebug("Launching conversion warning"));
            QProcess::startDetached(CONVERSION_WARNING_CMDLINE);
            IFDEBUG(qDebug("Conversion warning launched"));
            break;
        case 1:
            IFDEBUG(qDebug("Launching multiRule warning"));
            QProcess::startDetached(MULTIRULE_WARNING_CMDLINE);
            IFDEBUG(qDebug("Multi-rule warning launched"));
            break;
        }
    }

    IFDEBUG(qDebug("main: starting"));
    int ret = a.exec();
    IFDEBUG(qDebug("main: Exiting\n"));

    PlatformUtil::deinitialize();

    return ret;
}
