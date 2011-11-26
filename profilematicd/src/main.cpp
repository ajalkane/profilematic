#include <QtCore/QCoreApplication>
#include <QDBusMetaType>

#include "profileclient.h"
#include "configuration.h"
#include "preferences.h"
#include "model/rule.h"
#include "logic/rulewatch.h"
#include "logic/ruleactivator.h"
#include "interface/profilematicinterface.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ProfileClient profileClient;
    Preferences preferences;
    QList<Rule> rules;

    Configuration::readRules(rules);
    Configuration::readPreferences(preferences);
    RuleWatch ruleWatch(&rules, &preferences);
    RuleActivator ruleActivator(&profileClient);
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
    return ret;
}
