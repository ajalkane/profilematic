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

#include <QString>
#include <QObject>
#include <QList>

#include "profileclient.h"
#include "profilematicclient.h"

#include "qmlbackend/qmlbackend.h"
#include "qmlbackend/qmldaysmodel.h"
#include "qmlbackend/qmlrulesmodel.h"
#include "qmlbackend/qmlconditioneditmodel.h"
#include "qmlbackend/qmlactioneditmodel.h"
#include "qmlbackend/qmlboolfiltermodel.h"
#include "qmlbackend/qmlprofilesmodel.h"
#include "qmlbackend/qmlrulesummary.h"
#include "qmlbackend/qmllocation.h"
#include "qmlbackend/nfc/qmlnfcmobility.h"

#include "qmlapplicationviewer.h"

#include <qplatformdefs.h>
#ifdef MEEGO_EDITION_HARMATTAN
#include "qmlbackend/presence/qmlpresencemodelimpl.h"
#else
#include "qmlbackend/presence/qmlpresencemodelstub.h"
#endif

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QString mainQmlFile(QLatin1String("qml/main.qml"));
    if (argc >= 2) {
        if (QLatin1String("-conversionWarning") == argv[1]) {
            qDebug("Displaying conversion warning");
            mainQmlFile = QLatin1String("qml/mainConversionWarning.qml");
        } else if (QLatin1String("-multiRuleWarning") == argv[1]) {
            qDebug("Displaying multi-rule warning");
            mainQmlFile = QLatin1String("qml/mainMultiRuleWarning.qml");
        } else if (QLatin1String("-credentialWarning") == argv[1]) {
            qDebug("Displaying credential warning");
            mainQmlFile = QLatin1String("qml/mainCredentialWarning.qml");
        }
    }

    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    ProfileClient profileClient;
    ProfileMaticClient profileMaticClient;

    QmlProfilesModel qmlProfilesModel(&profileClient, QmlProfilesModel::ProfileType);
    QmlRuleSummary::initialize(&qmlProfilesModel);
    QmlDaysModel qmlDaysModel;
    QmlRulesModel qmlRulesModel(&profileMaticClient, &qmlProfilesModel);

    QmlConditionEditModel qmlConditionEditModel(qmlRulesModel.getEditRule());
    QmlBoolFilterModel qmlConditionEditVisibleModel(&qmlConditionEditModel, QmlConditionEditModel::VisibleRole, false);
    QmlBoolFilterModel qmlConditionEditNonVisibleModel(&qmlConditionEditModel, QmlConditionEditModel::VisibleRole, true);

    QmlActionEditModel qmlActionEditModel(qmlRulesModel.getEditRule());
    QmlBoolFilterModel qmlActionEditVisibleModel(&qmlActionEditModel, QmlConditionEditModel::VisibleRole, false);
    QmlBoolFilterModel qmlActionEditNonVisibleModel(&qmlActionEditModel, QmlConditionEditModel::VisibleRole, true);

    QmlBackend qmlBackend;
    QScopedPointer<QmlLocation> qmlLocation(QmlLocation::create());
    QmlNfcMobility qmlNfc;

    QDeclarativeContext *ctxt = viewer->rootContext();
    QDeclarativeEngine *engine = ctxt->engine();
    engine->connect(engine, SIGNAL(quit()), app.data(), SLOT(quit()));

    qmlRegisterType<Rule>("Rule", 1, 0, "Rule");
    qmlRegisterType<RuleCondition>("Rule", 1, 0, "RuleCondition");
    qmlRegisterType<RuleAction>("Rule", 1, 0, "RuleAction");
    qmlRegisterType<RuleConditionNFC>("Rule", 1, 0, "RuleConditionNFC");
    qmlRegisterType<PresenceRule>("profilematic", 1, 0, "PresenceRule");
#ifdef MEEGO_EDITION_HARMATTAN
    qmlRegisterType<QmlPresenceModelImpl>("profilematic", 1, 0, "AccountsModel");
#else
    qmlRegisterType<QmlPresenceModelStub>("profilematic", 1, 0, "AccountsModel");
#endif
    ctxt->setContextProperty("profileClient", &profileClient);
    ctxt->setContextProperty("backend", &qmlBackend);
    ctxt->setContextProperty("backendDaysModel", &qmlDaysModel);
    ctxt->setContextProperty("backendRulesModel", &qmlRulesModel);
    ctxt->setContextProperty("backendProfilesModel", &qmlProfilesModel);
    ctxt->setContextProperty("backendConditionEditModel", &qmlConditionEditModel);
    ctxt->setContextProperty("backendConditionEditVisibleModel", &qmlConditionEditVisibleModel);
    ctxt->setContextProperty("backendConditionEditNonVisibleModel", &qmlConditionEditNonVisibleModel);
    ctxt->setContextProperty("backendActionEditModel", &qmlActionEditModel);
    ctxt->setContextProperty("backendActionEditVisibleModel", &qmlActionEditVisibleModel);
    ctxt->setContextProperty("backendActionEditNonVisibleModel", &qmlActionEditNonVisibleModel);
    ctxt->setContextProperty("backendLocation", qmlLocation.data());
    ctxt->setContextProperty("backendNfc", &qmlNfc);

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(mainQmlFile);
    viewer->showExpanded();

//    MeeGo::QmDeviceMode device;
//    qDebug("main: setting flight mode on");
//    device.setMode(MeeGo::QmDeviceMode::Flight);

    int retVal = app->exec();
    qDebug("Exiting with return value %d", retVal);
    return retVal;
}
