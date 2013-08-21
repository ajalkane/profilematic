/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
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
#ifndef QMLRULEUTIL_H
#define QMLRULEUTIL_H

#include <QObject>
#include <QString>

// IMPROVE need to have the Rule object in common sources or library.
#include "../../profilematicd/src/model/rule.h"

#include "qmlprofilesmodel.h"

class QmlRuleUtil : public QObject
{
    Q_OBJECT

    static QmlRuleUtil *_instance;
    QmlProfilesModel *_profilesModel;

    QmlRuleUtil(QmlProfilesModel *profilesModel);

    QString _secsToShortString(int secsInput, bool shorter);

public:
    static void initialize(QmlProfilesModel *profilesModel);
    static void deinitialize();

    static inline QmlRuleUtil *instance() {
        return _instance;
    }

    QString timeSummary(const RuleCondition *rule, const QString &nonUsable, bool inListing = false);
    void timeClear(RuleCondition *rule);
    QString daysSummaryText(const QSet<int> &days, bool inListing = false);
    QString cellLocationSummary(const RuleCondition *rule, const QString &nonUsable, bool inListing = false);
    void cellLocationClear(RuleCondition *rule);
    QString internetConnectionModeSummary(const RuleCondition *rule, const QString &nonUsable, bool inListing = false);
    void internetConnectionModeClear(RuleCondition *rule);
    QString wlanSummary(const RuleCondition *rule, const QString &nonUsable, bool inListing = false);
    void wlanClear(RuleCondition *rule);
    QString idleSummary(const RuleCondition *cond, const QString &nonUsable, bool inListing = false);
    Q_INVOKABLE QString idleSummary(RuleCondition *cond, const QString &nonUsable, bool inListing);
    Q_INVOKABLE void idleClear(RuleCondition *cond);
    QString nfcSummary(const RuleCondition *rule, const QString &nonUsable, bool inListing = false);
    void nfcClear(RuleCondition *rule);
    QString chargingSummary(const RuleCondition *rule, const QString &nonUsable, bool inListing = false);
    void chargingClear(RuleCondition *rule);
    QString batteryLevelSummary(const RuleCondition *cond, const QString &nonUsable, bool inListing = false);
    Q_INVOKABLE QString batteryLevelSummary(RuleCondition *cond, const QString &nonUsable, bool inListing);
    Q_INVOKABLE void batteryLevelClear(RuleCondition *cond);
    QString calendarSummary(const RuleCondition *rule, const QString &nonUsable, bool inListing = false);
    void calendarClear(RuleCondition *rule);
    QString timeIntervalSummary(const RuleCondition *cond, const QString &nonUsable, bool inListing = false);
    Q_INVOKABLE QString timeIntervalActiveSummary(RuleCondition *cond, const QString &nonUsable);
    Q_INVOKABLE QString timeIntervalInactiveSummary(RuleCondition *cond, const QString &nonUsable);
    Q_INVOKABLE QString timeIntervalSummary(RuleCondition *cond, const QString &nonUsable, bool inListing);
    Q_INVOKABLE void timeIntervalClear(RuleCondition *cond);

    QString profileSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void profileClear(RuleAction *action);
    QString presenceSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void presenceClear(RuleAction *action);
    QString flightModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void flightModeClear(RuleAction *action);
    QString powerSavingModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void powerSavingModeClear(RuleAction *action);
    QString bluetoothModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void bluetoothModeClear(RuleAction *action);
    QString cellularModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void cellularModeClear(RuleAction *action);
    QString standByScreenModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void standByScreenModeClear(RuleAction *action);
    QString customActionSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void customActionClear(RuleAction *action);
    QString applicationSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void applicationClear(RuleAction *action);
    QString deviceVolumeSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    Q_INVOKABLE QString deviceVolumeSummary(RuleAction *action, const QString &nonUsable, bool inListing);
    Q_INVOKABLE void deviceVolumeClear(RuleAction *action);
    Q_INVOKABLE QString alarmSummary(RuleAction *action, const QString &nonUsable, bool inListing);
    QString alarmSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    void alarmClear(RuleAction *action);    
    QString deviceBrightnessSummary(const RuleAction *action, const QString &nonUsable, bool inListing = false);
    Q_INVOKABLE QString deviceBrightnessSummary(RuleAction *action, const QString &nonUsable, bool inListing);
    Q_INVOKABLE void deviceBrightnessClear(RuleAction *action);

};

#endif // QMLRULEUTIL_H
