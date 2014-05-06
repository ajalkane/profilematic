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
#include <QTranslator>

#include <QtCore/QStringList>

#include <QString>
#include <QObject>
#include <QList>

#include "profileclient.h"
#include "profilematicclient.h"

#include "qmlbackend/qmlbackend.h"
#include "qmlbackend/qmlchoisemodel.h"
#include "qmlbackend/qmldaysmodel.h"
#include "qmlbackend/qmlrulesmodel.h"
#include "qmlbackend/qmlboolfiltermodel.h"
#include "qmlbackend/qmlsortproxymodel.h"
#include "qmlbackend/qmlprofilesmodel.h"
#include "qmlbackend/qmlruleutil.h"
#include "qmlbackend/qmllocation.h"
#include "qmlbackend/nfc/qmlnfcmobility.h"
#include "qmlbackend/networkinfo/qmlnetworkinfo.h"
#include "qmlbackend/application/qmlapplicationsmodel.h"
#include "qmlbackend/application/qmlapplicationsscanner.h"
#include "qmlbackend/application/qmlapplicationscannertomodeladapter.h"
#include "qmlbackend/application/qmlselectedapplicationsmodel.h"

#include "qmlapplicationviewer.h"

#include <qplatformdefs.h>
#ifdef MEEGO_EDITION_HARMATTAN
#include "qmlbackend/presence/qmlpresencemodelimpl.h"
#else
#include "qmlbackend/presence/qmlpresencemodelstub.h"
#endif

void setupEditModel(const QmlBaseRuleEditModel *qmlEditModel, QmlBoolFilterModel *qmlFilterModel);
void setupTranslations(QApplication *app, QTranslator *translator);

// IMPROVE: Too long and messy main function. Should chop it up in logical pieces.
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

    QTranslator translator;
    setupTranslations(app.data(), &translator);

    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    // Reduce flicker - maybe. I couldn't see any difference, but it didn't hurt
    // either. See http://blog.rburchell.com/2011/11/avoiding-graphics-flicker-in-qt-qml.html
    viewer->setAttribute(Qt::WA_OpaquePaintEvent);
    viewer->setAttribute(Qt::WA_NoSystemBackground);
    viewer->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    viewer->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    ProfileClient profileClient;
    ProfileMaticClient profileMaticClient;

    QmlProfilesModel qmlProfilesModel(&profileClient, QmlProfilesModel::ProfileType);
    QmlRuleUtil::initialize(&qmlProfilesModel);
    QmlDaysModel qmlDaysModel;
    QmlRulesModel qmlRulesModel(&profileMaticClient, &qmlProfilesModel);

    QmlBoolFilterModel qmlConditionEditVisibleModel(qmlRulesModel.getConditionEditModel(), QmlConditionEditModel::VisibleRole, false);
    QmlBoolFilterModel qmlConditionEditNonVisibleModel(qmlRulesModel.getConditionEditModel(), QmlConditionEditModel::VisibleRole, true);
    setupEditModel(qmlRulesModel.getConditionEditModel(), &qmlConditionEditVisibleModel);
    setupEditModel(qmlRulesModel.getConditionEditModel(), &qmlConditionEditNonVisibleModel);

    QmlBoolFilterModel qmlActionEditVisibleModel(qmlRulesModel.getActionEditModel(), QmlConditionEditModel::VisibleRole, false);
    QmlBoolFilterModel qmlActionEditNonVisibleModel(qmlRulesModel.getActionEditModel(), QmlConditionEditModel::VisibleRole, true);
    setupEditModel(qmlRulesModel.getActionEditModel(), &qmlActionEditVisibleModel);
    setupEditModel(qmlRulesModel.getActionEditModel(), &qmlActionEditNonVisibleModel);

    QmlBackend qmlBackend;
    QScopedPointer<QmlLocation> qmlLocation(QmlLocation::create());
    QmlNfcMobility qmlNfc;

    QmlApplicationsScanner qmlApplicationsScanner;
    QmlSelectedApplicationsModel qmlSelectedApplicationsModel(qmlRulesModel.getEditRule(), &qmlApplicationsScanner);
    QmlSortProxyModel qmlSelectedApplicationsSortedModel(&qmlSelectedApplicationsModel, QmlSelectedApplicationsModel::ApplicationName);
    QmlApplicationsModel qmlApplicationsModel;
    QmlApplicationScannerToModelAdapter qmlApplicationScannerToModelAdapter(&qmlApplicationsModel, &qmlApplicationsScanner);
    QmlSortProxyModel qmlApplicationsSortedModel(&qmlApplicationsModel, QmlApplicationsModel::ApplicationName);

    QDeclarativeContext *ctxt = viewer->rootContext();
    QDeclarativeEngine *engine = ctxt->engine();
    engine->connect(engine, SIGNAL(quit()), app.data(), SLOT(quit()));

    qmlRegisterType<Rule>("Rule", 1, 0, "Rule");
    qmlRegisterType<RuleCondition>("Rule", 1, 0, "RuleCondition");
    qmlRegisterType<RuleAction>("Rule", 1, 0, "RuleAction");
    qmlRegisterType<RuleActionApplication>("Rule", 1, 0, "RuleActionApplication");
    qmlRegisterType<RuleActionAlarm>("Rule", 1, 0, "RuleActionAlarm");
    qmlRegisterType<RuleConditionNFC>("Rule", 1, 0, "RuleConditionNFC");
    qmlRegisterType<RuleConditionBatteryLevel>("Rule", 1, 0, "RuleConditionBatteryLevel");
    qmlRegisterType<RuleConditionCalendar>("Rule", 1, 0, "RuleConditionCalendar");
    qmlRegisterType<RuleConditionTimeInterval>("Rule", 1, 0, "RuleConditionTimeInterval");
    qmlRegisterType<PresenceRule>("profilematic", 1, 0, "PresenceRule");
    qmlRegisterType<QmlNetworkInfo>("profilematic", 1, 0, "NetworkInfo");
    qmlRegisterType<QmlChoiseModel>("profilematic", 1, 0, "ChoiseModel");
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
    ctxt->setContextProperty("backendConditionEditModel", qmlRulesModel.getConditionEditModel());
    ctxt->setContextProperty("backendConditionEditVisibleModel", &qmlConditionEditVisibleModel);
    ctxt->setContextProperty("backendConditionEditNonVisibleModel", &qmlConditionEditNonVisibleModel);
    ctxt->setContextProperty("backendActionEditModel", qmlRulesModel.getActionEditModel());
    ctxt->setContextProperty("backendActionEditVisibleModel", &qmlActionEditVisibleModel);
    ctxt->setContextProperty("backendActionEditNonVisibleModel", &qmlActionEditNonVisibleModel);
    ctxt->setContextProperty("backendLocation", qmlLocation.data());
    ctxt->setContextProperty("backendNfc", &qmlNfc);
    ctxt->setContextProperty("backendRuleUtil", QmlRuleUtil::instance());
    ctxt->setContextProperty("backendApplicationsModel", &qmlApplicationsModel);
    ctxt->setContextProperty("backendApplicationsSortedModel", &qmlApplicationsSortedModel);
    ctxt->setContextProperty("backendApplicationsScanner", &qmlApplicationScannerToModelAdapter);
    ctxt->setContextProperty("backendSelectedApplicationsModel", &qmlSelectedApplicationsModel);
    ctxt->setContextProperty("backendSelectedApplicationsSortedModel", &qmlSelectedApplicationsSortedModel);

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(mainQmlFile);
    viewer->showExpanded();

    int retVal = app->exec();
    qDebug("Exiting with return value %d", retVal);
    QmlRuleUtil::deinitialize();

    return retVal;
}

void setupEditModel(const QmlBaseRuleEditModel *qmlEditModel, QmlBoolFilterModel *qmlFilterModel) {
    qmlEditModel->connect(qmlEditModel, SIGNAL(visibleChanged()), qmlFilterModel, SLOT(invalidate()));
    qmlEditModel->connect(qmlEditModel, SIGNAL(visibleChanged()), qmlFilterModel, SLOT(invalidate()));

    qmlFilterModel->setSortRole(QmlBaseRuleEditModel::TopicRole);
    qmlFilterModel->sort(0);
}

QString adjustPath(const QString &path)
{
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
    if (!QDir::isAbsolutePath(path))
        return QString::fromLatin1("%1/../Resources/%2")
                .arg(QCoreApplication::applicationDirPath(), path);
#else
    const QString pathInInstallDir =
            QString::fromLatin1("%1/../%2").arg(QCoreApplication::applicationDirPath(), path);
    qDebug() << Q_FUNC_INFO << "got candidate" << pathInInstallDir;
    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
#endif
#endif
    qDebug() << Q_FUNC_INFO << "returning default" << path;
    return path;
}

void setupTranslations(QApplication *app, QTranslator *translator) {
    QString locale = QLocale::system().name();
    QString translationsDir = adjustPath("i18n");
    qDebug() << "Locale:" << locale << " translationsDir:" << translationsDir;

    // fall back to using English translation, if one specific to the current
    // setting of the device is not available.
    if (!(translator->load("tr_"+locale, translationsDir))) {
        qDebug() << "Translation for locale " << locale << " not found, loading default english";
        if (!translator->load("tr_en", translationsDir)) {
            qWarning() << "Translation for default english not found either. Annoying.";
        } else {
            qDebug() << "Default english translation used for locale " << locale;
        }
    } else {
        qDebug() << "Found translation used for locale " << locale;
    }
    app->installTranslator(translator);
}
