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

#include <QtGui/QApplication>
#include <QtDeclarative>

#include <QtCore/QStringList>
#include <QtDBus/QtDBus>

#include <QObject>
#include <QList>

#include "profileclient.h"
#include "profilematicclient.h"
//#include "settings.h"
//#include "logic/rulewatch.h"
//#include "logic/ruleactivator.h"

#include "qmlbackend/qmlbackend.h"
#include "qmlbackend/qmldaysmodel.h"
#include "qmlbackend/qmlrulesmodel.h"
#include "qmlbackend/qmlprofilesmodel.h"

#include "qmlapplicationviewer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    qDBusRegisterMetaType<Rule>();
    qDBusRegisterMetaType<QList<Rule> >();

    ProfileClient profileClient;
    ProfileMaticClient profileMaticClient;
    QList<Rule> rules = profileMaticClient.getRules();
    // Settings::read(rules);
    // RuleWatch ruleWatch(&rules);
    // RuleActivator ruleActivator(&profileClient);

    //ruleActivator.connect(&ruleWatch, SIGNAL(activateRule(RuleItem)), &ruleActivator, SLOT(activateRule(RuleItem)));

    // ruleWatch.refreshWatch();

    QmlProfilesModel qmlProfilesModel(&profileClient, QmlProfilesModel::ProfileType);
    QmlDaysModel qmlDaysModel;
    QmlRulesModel qmlRulesModel(rules);
    QmlBackend qmlBackend;

    QDeclarativeContext *ctxt = viewer->rootContext();

    ctxt->setContextProperty("profileClient", &profileClient);
    ctxt->setContextProperty("backend", &qmlBackend);
    ctxt->setContextProperty("backendDaysModel", &qmlDaysModel);
    ctxt->setContextProperty("backendRulesModel", &qmlRulesModel);
    ctxt->setContextProperty("backendProfilesModel", &qmlProfilesModel);

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String("qml/main.qml"));
    viewer->showExpanded();

    int retVal = app->exec();
    qDebug("Exiting with return value %d", retVal);
    return retVal;
}
