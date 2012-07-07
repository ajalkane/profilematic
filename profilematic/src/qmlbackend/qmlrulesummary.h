#ifndef QMLRULESUMMARY_H
#define QMLRULESUMMARY_H

#include <QString>

// IMPROVE need to have the Rule object in common sources or library.
#include "../../profilematicd/src/model/rule.h"

class QmlRuleSummary
{
public:
    QmlRuleSummary();

    static QString timeSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString daysSummaryText(const QSet<int> &days);
    static QString cellLocationSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString internetConnectionModeSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString wlanSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString idleSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString nfcSummary(const RuleCondition *rule, const QString &nonUsable);
    static QString chargingSummary(const RuleCondition *rule, const QString &nonUsable);
};

#endif // QMLRULESUMMARY_H
