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

#include <QString>

// IMPROVE need to have the Rule object in common sources or library.
#include "../../profilematicd/src/model/rule.h"

#include "qmlprofilesmodel.h"

class QmlRuleUtil
{
    static QmlRuleUtil *_instance;
    QmlProfilesModel *_profilesModel;

    QmlRuleUtil(QmlProfilesModel *profilesModel);
public:
    static void initialize(QmlProfilesModel *profilesModel);
    static void deinitialize();

    static inline QmlRuleUtil *instance() {
        return _instance;
    }

    QString timeSummary(const RuleCondition *rule, const QString &nonUsable);
    void timeClear(RuleCondition *rule);
    QString daysSummaryText(const QSet<int> &days);
    QString cellLocationSummary(const RuleCondition *rule, const QString &nonUsable);
    void cellLocationClear(RuleCondition *rule);
    QString internetConnectionModeSummary(const RuleCondition *rule, const QString &nonUsable);
    void internetConnectionClear(RuleCondition *rule);
    QString wlanSummary(const RuleCondition *rule, const QString &nonUsable);
    void wlanClear(RuleCondition *rule);
    QString idleSummary(const RuleCondition *rule, const QString &nonUsable);
    void idleClear(RuleCondition *rule);
    QString nfcSummary(const RuleCondition *rule, const QString &nonUsable);
    void nfcClear(RuleCondition *rule);
    QString chargingSummary(const RuleCondition *rule, const QString &nonUsable);
    void chargingClear(RuleCondition *rule);

    QString profileSummary(const RuleAction *action, const QString &nonUsable);
    void profileClear(RuleAction *action);
    QString presenceSummary(const RuleAction *action, const QString &nonUsable);
    void presenceClear(RuleAction *action);
    QString flightModeSummary(const RuleAction *action, const QString &nonUsable);
    void flightModeClear(RuleAction *action);
    QString powerSavingModeSummary(const RuleAction *action, const QString &nonUsable);
    void powerSavingClear(RuleAction *action);
    QString bluetoothModeSummary(const RuleAction *action, const QString &nonUsable);
    void bluetoothClear(RuleAction *action);
    QString cellularModeSummary(const RuleAction *action, const QString &nonUsable);
    void cellularModeClear(RuleAction *action);
    QString standByScreenModeSummary(const RuleAction *action, const QString &nonUsable);
    void standByScreenModeClear(RuleAction *action);
    QString customActionSummary(const RuleAction *action, const QString &nonUsable);
    void customActionClear(RuleAction *action);
};

#endif // QMLRULEUTIL_H