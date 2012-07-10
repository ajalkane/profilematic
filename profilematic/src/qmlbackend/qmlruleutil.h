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
    QString daysSummaryText(const QSet<int> &days);
    QString cellLocationSummary(const RuleCondition *rule, const QString &nonUsable);
    QString internetConnectionModeSummary(const RuleCondition *rule, const QString &nonUsable);
    QString wlanSummary(const RuleCondition *rule, const QString &nonUsable);
    QString idleSummary(const RuleCondition *rule, const QString &nonUsable);
    QString nfcSummary(const RuleCondition *rule, const QString &nonUsable);
    QString chargingSummary(const RuleCondition *rule, const QString &nonUsable);

    QString profileSummary(const RuleAction *action, const QString &nonUsable);
    QString presenceSummary(const RuleAction *action, const QString &nonUsable);
    QString flightModeSummary(const RuleAction *action, const QString &nonUsable);
    QString powerSavingModeSummary(const RuleAction *action, const QString &nonUsable);
    QString bluetoothModeSummary(const RuleAction *action, const QString &nonUsable);
    QString cellularModeSummary(const RuleAction *action, const QString &nonUsable);
    QString standByScreenModeSummary(const RuleAction *action, const QString &nonUsable);
    QString customActionSummary(const RuleAction *action, const QString &nonUsable);
};

#endif // QMLRULEUTIL_H
