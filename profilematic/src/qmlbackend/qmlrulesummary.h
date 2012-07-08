#ifndef QMLRULESUMMARY_H
#define QMLRULESUMMARY_H

#include <QString>

// IMPROVE need to have the Rule object in common sources or library.
#include "../../profilematicd/src/model/rule.h"

#include "qmlprofilesmodel.h"

class QmlRuleSummary
{
    static QmlProfilesModel *_profilesModel;

public:
    static inline void initialize(QmlProfilesModel *profilesModel) {
        _profilesModel = profilesModel;
    }

    static QString timeSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString daysSummaryText(const QSet<int> &days);
    static QString cellLocationSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString internetConnectionModeSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString wlanSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString idleSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString nfcSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString chargingSummary(const RuleCondition *rule, const QString &nonUsable);

    static QString profileSummary(const RuleAction *action, const QString &nonUsable);
    static QString presenceSummary(const RuleAction *action, const QString &nonUsable);
    static QString flightModeSummary(const RuleAction *action, const QString &nonUsable);
    static QString powerSavingModeSummary(const RuleAction *action, const QString &nonUsable);
    static QString bluetoothModeSummary(const RuleAction *action, const QString &nonUsable);
    static QString cellularModeSummary(const RuleAction *action, const QString &nonUsable);
    static QString standByScreenModeSummary(const RuleAction *action, const QString &nonUsable);
    static QString customActionSummary(const RuleAction *action, const QString &nonUsable);
};

#endif // QMLRULESUMMARY_H
