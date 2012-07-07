#include "qmlrulesummary.h"

QmlRuleSummary::QmlRuleSummary()
{
}

QString
QmlRuleSummary::timeSummary(const RuleCondition *rule, const QString &nonUsableTimeString) {
    // qDebug("QmlRulesModel::getTimeSummaryText()");
    // Rule rule = ruleVariant.value<Rule>();
    if (rule == 0) {
        qDebug("QmlRulesModel::getTimeSummaryText() null rule");
        return nonUsableTimeString;
    }

    if (rule->getDays().isEmpty()
            || !rule->getTimeStart().isValid()
            || !rule->getTimeEnd().isValid()) {
//        qDebug("QmlRulesModel::getTimeSummaryText(): nonUsable, getDays.isEmpty/!validTimeStart/!validTimeEnd %d, %d, %d",
//               rule->getDays().isEmpty(), !rule->getTimeStart().isValid(), !rule->getTimeEnd().isValid());
        return nonUsableTimeString;
    }

    QString summary;
    summary += rule->getTimeStartQml();
    summary += " - ";
    summary += rule->getTimeEndQml();
    if (rule->getTimeStart() == rule->getTimeEnd()) {
        summary += " (";
        summary += "24h";
        summary += ")";
    }
    // I don't think this length clarification is needed otherwise
//    else {
//        QTime duration = rule->getTimeDuration();
//        qDebug("QmlRulesModel::getTimeSummaryText() 3" );
//        if (duration.hour() != 0) {
//            summary += QString::number(duration.hour());
//            summary += "h";
//        }
//        qDebug("QmlRulesModel::getTimeSummaryText() 3");
//        if (duration.minute() != 0) {
//            summary += QString::number(duration.minute());
//            summary += "min";
//        }
//    }

    summary += " ";
//    qDebug("QmlRulesModel::getTimeSummaryText() getDaysSummaryText");
    summary += daysSummaryText(rule->getDays());
//    qDebug("/QmlRulesModel::getTimeSummaryText() returning %s", qPrintable(summary));

    return summary;
}

QString
QmlRuleSummary::daysSummaryText(const QSet<int> &days) {
    // qDebug("QmlRulesModel::getDaysSummaryText(), days size %d", days.size());

    if (days.size() == 7) {
        return "All days";
    }
    if (days.size() == 0) {
        return "No days";
    }
    // If only one day selected, display full name
    if (days.size() == 1) {
        int dayId = *(days.constBegin());
        return QDate::longDayName(dayId + 1, QDate::StandaloneFormat);
    }

    int rangeStart = -1;

    QString daysStr = "";
    // The loop goes up-to 7 so that Sunday is considered as last in range.
    // Days.contains for 7 should always contain false as days are in range of 0-6.
    for (int i = 0; i < 8; ++i) {
        if (days.contains(i)) {
            if (rangeStart < 0) {
                rangeStart = i;

            }
        } else {
            if (rangeStart > -1) {
                if (!daysStr.isEmpty()) {
                    daysStr += ", ";
                }

                daysStr += QDate::shortDayName(rangeStart + 1, QDate::StandaloneFormat);
                if (rangeStart < i - 1) {
                    daysStr += " - ";
                    daysStr += QDate::shortDayName((i - 1) + 1, QDate::StandaloneFormat);
                }
                rangeStart = -1;
            }
        }
    }
    return daysStr;
}

QString
QmlRuleSummary::cellLocationSummary(const RuleCondition *cond, const QString &nonUsable) {
    if (cond == 0) return nonUsable;

    int numCellIds = cond->getLocationCells().size();
    QString s;
    if (numCellIds == 0) {
        s.append(nonUsable);
    } else {
        s.append("Cell ids set");
        if (cond->getLocationCellsTimeout() > 0) {
            s.append(QString(" (%1 timeout)").arg(cond->getLocationCellsTimeout()));
        }
    }
    return s;
}

QString
QmlRuleSummary::internetConnectionModeSummary(const RuleCondition *cond, const QString &nonUsable) {
    if (cond == 0) return nonUsable;

    switch (cond->getInternetConnectionMode()) {
    case RuleCondition::Gsm:
        return "Mobile";
    case RuleCondition::Wlan:
        return "WLAN";
    default:
        return nonUsable;
    }
    return nonUsable;
}

QString
QmlRuleSummary::wlanSummary(const RuleCondition *cond, const QString &nonUsable) {
    if (cond == 0) return nonUsable;

    int numWlans = cond->getWlan().size();
    QString s;
    if (numWlans == 0) {
        s.append(nonUsable);
    } else {
        s.append("In use");
        if (cond->getWlanTimeout() > 0) {
            s.append(QString(" (%1 timeout)").arg(cond->getWlanTimeout()));
        }
    }
    return s;
}

QString
QmlRuleSummary::idleSummary(const RuleCondition *cond, const QString &nonUsable) {
    if (cond == 0) return nonUsable;

    int idleForSecs = cond->getIdleForSecs();
    QString s;
    if (idleForSecs < 0) {
        s.append(nonUsable);
    } else {
        s.append(QString("At least for %1 minutes").arg(idleForSecs / 60));
    }
    return s;
}

QString
QmlRuleSummary::nfcSummary(const RuleCondition *cond, const QString &nonUsable) {
    if (cond == 0) return nonUsable;

    int nfcUids = cond->nfc().getUids().size();
    QString s;
    if (nfcUids == 0) {
        s.append(nonUsable);
    } else {
        s.append("NFC detections set");
        if (cond->nfc().getToggleCondition()) {
            s.append(" (toggles)");
        }
    }
    return s;
}

QString
QmlRuleSummary::chargingSummary(const RuleCondition *cond, const QString &nonUsable) {
    if (cond == 0) return nonUsable;

    RuleCondition::ChargingState chargingState = cond->getChargingState();
    QString s;
    switch (chargingState) {
    case RuleCondition::NotCharging:
        s.append("Not charging power");
        break;
    case RuleCondition::Charging:
        s.append("Charging power");
        break;
    default:
        s.append(nonUsable);
        break;
    }

    return s;
}
