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
#include <QDebug>

#include <cstdio>
#include <cstdlib>
#include "qmlruleutil.h"

QmlRuleUtil *QmlRuleUtil::_instance = 0;

void
QmlRuleUtil::initialize(QmlProfilesModel *profilesModel) {
    if (_instance != 0) {
        fprintf(stderr, "QmlRuleUtil::initialize called more than once");
        exit(1);
    }

    _instance = new QmlRuleUtil(profilesModel);
}

void
QmlRuleUtil::deinitialize() {
    delete _instance;
    _instance = 0;
}

QmlRuleUtil::QmlRuleUtil(QmlProfilesModel *profilesModel) : QObject(0) {
    _profilesModel = profilesModel;
}

QString
QmlRuleUtil::timeSummary(const RuleCondition *cond, const QString &nonUsableTimeString, bool /*inListing*/) {
    // qDebug("QmlRulesModel::getTimeSummaryText()");
    // Rule rule = ruleVariant.value<Rule>();
    if (cond == 0) {
        qDebug("QmlRulesModel::getTimeSummaryText() null rule");
        return nonUsableTimeString;
    }

    if (cond->getDays().isEmpty()
            || !cond->getTimeStart().isValid()
            || !cond->getTimeEnd().isValid()) {
//        qDebug("QmlRulesModel::getTimeSummaryText(): nonUsable, getDays.isEmpty/!validTimeStart/!validTimeEnd %d, %d, %d",
//               rule->getDays().isEmpty(), !rule->getTimeStart().isValid(), !rule->getTimeEnd().isValid());
        return nonUsableTimeString;
    }

    QString summary;
    summary += cond->getTimeStartQml();
    summary += tr(" - ", "This a range separator between time start and time end. Notice space separators before and after the - sign, they should be in translation too unless there's good reason");
    summary += cond->getTimeEndQml();
    if (cond->getTimeStart() == cond->getTimeEnd()) {
        summary += " (";
        summary += tr("24h", "Time condition length");
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
    summary += daysSummaryText(cond->getDays());
//    qDebug("/QmlRulesModel::getTimeSummaryText() returning %s", qPrintable(summary));

    return summary;
}

void
QmlRuleUtil::timeClear(RuleCondition *cond) {
    if (cond == 0) return;

    QSet<int> days;
    cond->setDays(days);
    cond->setTimeStart(QTime());
    cond->setTimeEnd(QTime());
}

QString
QmlRuleUtil::daysSummaryText(const QSet<int> &days, bool /*inListing*/) {
    // qDebug("QmlRulesModel::getDaysSummaryText(), days size %d", days.size());

    if (days.size() == 7) {
        return tr("All days");
    }
    if (days.size() == 0) {
        return tr("No days");
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
                    daysStr += tr(", ");
                }

                daysStr += QDate::shortDayName(rangeStart + 1, QDate::StandaloneFormat);
                if (rangeStart < i - 1) {
                    daysStr += tr(" - ", "This a range separator between day start and day end. Notice space separators before and after the - sign, they should be in translation too unless there's good reason");
                    daysStr += QDate::shortDayName((i - 1) + 1, QDate::StandaloneFormat);
                }
                rangeStart = -1;
            }
        }
    }
    return daysStr;
}

QString
QmlRuleUtil::cellLocationSummary(const RuleCondition *cond, const QString &nonUsable, bool inListing) {
    if (cond == 0) return nonUsable;

    int numCellIds = cond->getLocationCells().size();
    QString s;
    if (numCellIds == 0) {
        s.append(nonUsable);
    } else {
        if (inListing) {
            s.append(tr("on location"));
        } else {
            s.append(tr("Cell ids set"));
            if (cond->getLocationCellsTimeout() > 0) {
                s.append(tr(" (%1s timeout)", " (number of seconds timeout, please preserve the space before the translated string)").arg(cond->getLocationCellsTimeout()));
            }
        }
    }
    return s;
}

void
QmlRuleUtil::cellLocationClear(RuleCondition *cond) {
    if (cond == 0) return;

    QSet<int> cells;
    cond->setLocationCells(cells);
    cond->setLocationCellsTimeout(0);
}

QString
QmlRuleUtil::internetConnectionModeSummary(const RuleCondition *cond, const QString &nonUsable, bool /* inListing */) {
    if (cond == 0) return nonUsable;

    switch (cond->getInternetConnectionMode()) {
    case RuleCondition::Gsm:
        return tr("Mobile net connection");
    case RuleCondition::Wlan:
        return tr("WLAN net connection");
    case RuleCondition::ConnectionAny:
        return tr("Any net connection");
    case RuleCondition::ConnectionNone:
        return tr("No net connection");
    default:
        return nonUsable;
    }
    return nonUsable;
}

void
QmlRuleUtil::internetConnectionModeClear(RuleCondition *cond) {
    if (cond == 0) return;

    cond->setInternetConnectionMode(RuleCondition::UndefinedInternetConnectionMode);
}

QString
QmlRuleUtil::wlanSummary(const RuleCondition *cond, const QString &nonUsable, bool inListing) {
    if (cond == 0) return nonUsable;

    int numWlans = cond->getWlan().size();
    QString s;
    if (numWlans == 0) {
        s.append(nonUsable);
    } else {
        if (inListing) {
            s.append(tr("on WLAN"));
        } else {
            s.append(tr("In use"));
            if (cond->getWlanTimeout() > 0) {
                s.append(tr(" (%1s timeout)", " (number of seconds timeout, please preserve the space before the translated string)").arg(cond->getWlanTimeout()));
            }
        }
    }
    return s;
}

void
QmlRuleUtil::wlanClear(RuleCondition *cond) {
    if (cond == 0) return;

    QSet<QString> numWlans;
    cond->setWlan(numWlans);
    cond->setWlanTimeout(0);
}

QString
QmlRuleUtil::idleSummary(const RuleCondition *cond, const QString &nonUsable, bool inListing) {
    if (cond == 0) return nonUsable;

    int idleForSecs = cond->getIdleForSecs();
    QString s;
    if (idleForSecs < 0) {
        s.append(nonUsable);
    } else {
        if (inListing) {
            s.append(tr("Idle"));
        } else {
            s.append(tr("At least for %1 minutes").arg(idleForSecs / 60));
        }
    }
    return s;
}

void
QmlRuleUtil::idleClear(RuleCondition *cond) {
    if (cond == 0) return;

    cond->setIdleForSecs(-1);
}

QString
QmlRuleUtil::nfcSummary(const RuleCondition *cond, const QString &nonUsable, bool inListing) {
    if (cond == 0) return nonUsable;

    int nfcUids = cond->nfc().getUids().size();
    QString s;
    if (nfcUids == 0) {
        s.append(nonUsable);
    } else {
        if (inListing) {
            s.append(tr("NFC"));
        } else {
            s.append(tr("NFC detections set"));
            if (cond->nfc().getToggleCondition()) {
                s.append(tr(" (toggles)", "Please notice the space in the beginning of translation, and use appropriate separator character in translation"));
            }
        }
    }
    return s;
}

void
QmlRuleUtil::nfcClear(RuleCondition *cond) {
    if (cond == 0) return;

    QSet<QByteArray> uids;
    cond->nfc().setUids(uids);
    cond->nfc().setToggleCondition(false);
}


QString
QmlRuleUtil::chargingSummary(const RuleCondition *cond, const QString &nonUsable, bool inListing) {
    if (cond == 0) return nonUsable;

    RuleCondition::ChargingState chargingState = cond->getChargingState();
    QString s;
    switch (chargingState) {
    case RuleCondition::NotCharging:
        s.append(inListing ? tr("not charging") : tr("Not charging power"));
        break;
    case RuleCondition::Charging:
        s.append(inListing ? tr("charging") : tr("Charging power"));
        break;
    default:
        s.append(nonUsable);
        break;
    }

    return s;
}

void
QmlRuleUtil::chargingClear(RuleCondition *cond) {
    if (cond == 0) return;

    cond->setChargingState(RuleCondition::UndefinedChargingState);
}

QString
QmlRuleUtil::batteryLevelSummary(RuleCondition *cond, const QString &nonUsable, bool inListing) {
    qDebug("batteryLevelSummary QML version");
    return batteryLevelSummary(const_cast<const RuleCondition *>(cond), nonUsable, inListing);
}

QString
QmlRuleUtil::batteryLevelSummary(const RuleCondition *cond, const QString &nonUsable, bool /*inListing*/) {
    if (cond == 0) return nonUsable;

    const RuleConditionBatteryLevel &bl = cond->batteryLevel();
    if (!bl.isValid()) {
        return nonUsable;
    }
    qDebug("QmlRuleUtil::Battery level %d - %d", bl.getLevelMin(), bl.getLevelMax());

    if (bl.getLevelMax() == bl.getLevelMin()) {
        return tr("Battery level exactly %1%").arg(bl.getLevelMin());
    }
    else if (bl.getLevelMax() == 100) {
        return tr("Battery level at least %1%").arg(bl.getLevelMin());
    } else if (bl.getLevelMin() == 0) {
        return tr("Battery level at most %1%").arg(bl.getLevelMax());
    }
    return tr("Battery level between %1% - %2%").arg(bl.getLevelMin()).arg(bl.getLevelMax());
}

void
QmlRuleUtil::batteryLevelClear(RuleCondition *cond) {
    if (cond == 0) return;

    cond->batteryLevel().clear();
}

QString
QmlRuleUtil::calendarSummary(const RuleCondition *cond, const QString &nonUsable, bool inListing) {
    Q_UNUSED(inListing)

    if (cond == 0) return nonUsable;

    const RuleConditionCalendar &calendar = cond->calendar();
    if (calendar.isValid()) {
        return tr("Calendar entries");
    }
    return nonUsable;
}

void
QmlRuleUtil::calendarClear(RuleCondition *cond) {
    if (cond == 0) return;

    cond->calendar().clear();
}

QString
QmlRuleUtil::profileSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    if (action == 0) return nonUsable;

    QString profile = action->getProfile();
    if (!profile.isEmpty()) {
        QString profileName = _profilesModel->getProfileToName(profile);
        QString summary = profileName;

        if (!inListing) {
            if (_profilesModel->profileHasVolume(profile) && action->getProfileVolume() > -1) {
                summary += tr(" (%1%)", "Please notice the space in the beginning of translation, and use appropriate separator character in translation").arg(action->getProfileVolume());
            }
            if (action->getRestoreProfile()) {
                summary += tr(". Restores previous profile.");
            }
        }
        return summary;
    }
    return nonUsable;
}

void
QmlRuleUtil::profileClear(RuleAction *action) {
    if (action == 0) return;

    action->setProfile(QString());
    action->setProfileVolume(-1);
    action->setRestoreProfile(false);
}

QString
QmlRuleUtil::presenceSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    if (action == 0) return nonUsable;

    QString summary = nonUsable;
    bool atLeastOneChange = false;

    foreach (PresenceRule *presenceRule, action->presenceRules()) {
        if (presenceRule->action() != PresenceRule::Retain) {
            atLeastOneChange = true;
            break;
        }
    }

    if (atLeastOneChange)
        summary = tr("Availability change");

    if (!inListing && action->getRestorePresence())
        summary += tr(". Restores previous availability");

    return summary;
}

void
QmlRuleUtil::presenceClear(RuleAction *action) {
    if (action == 0) return;

    QList<PresenceRule *> presenceRules;
    action->setPresenceRules(presenceRules);
}

QString
QmlRuleUtil::flightModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    if (action == 0) return nonUsable;

    QString summary;
    switch (action->getFlightMode()) {
    case 0:
        summary = tr("Flight mode off"); break;
    case 1:
        summary = tr("Flight mode on"); break;
    default:
        return nonUsable;
    }

    if (!inListing && action->getRestoreFlightMode()) {
        summary += tr(". Restores previous mode.");
    }
    return summary;
}

void
QmlRuleUtil::flightModeClear(RuleAction *action) {
    if (action == 0) return;

    action->setFlightMode(-1);
    action->setRestoreFlightMode(false);
}

QString
QmlRuleUtil::powerSavingModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    if (action == 0) return nonUsable;

    QString summary;
    switch (action->getPowerSavingMode()) {
    case 0:
        summary = tr("Power saving off"); break;
    case 1:
        summary = tr("Power saving on"); break;
    default:
        return nonUsable;
    }

    if (!inListing && action->getRestorePowerSavingMode()) {
        summary += tr(". Restores previous mode.");
    }
    return summary;
}

void
QmlRuleUtil::powerSavingModeClear(RuleAction *action) {
    if (action == 0) return;

    action->setPowerSavingMode(-1);
    action->setRestorePowerSavingMode(false);
}

QString
QmlRuleUtil::bluetoothModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    if (action == 0) return nonUsable;

    QString summary;
    switch (action->getBlueToothMode()) {
    case 0:
        summary = tr("Bluetooth off"); break;
    case 1:
        summary = tr("Bluetooth on"); break;
    case 2:
        summary = inListing ? tr("Bluetooth on") : tr("Bluetooth on and visible"); break;
    default:
        return nonUsable;
    }

    if (!inListing && action->getRestoreBlueToothMode()) {
        summary += tr(". Restores previous mode.");
    }
    return summary;
}

void
QmlRuleUtil::bluetoothModeClear(RuleAction *action) {
    if (action == 0) return;

    action->setBlueToothMode(-1);
    action->setRestoreBlueToothMode(false);
}

QString
QmlRuleUtil::cellularModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    if (action == 0) return nonUsable;

    QString summary;
    switch (action->getCellularMode()) {
    case 0:
        summary = tr("Dual GSM/3G"); break;
    case 1:
        summary = tr("GSM"); break;
    case 2:
        summary = tr("3G"); break;
    default:
        return nonUsable;
    }

    if (!inListing && action->getRestoreCellularMode()) {
        summary += tr(". Restores previous mode.");
    }

    return summary;
}

void
QmlRuleUtil::cellularModeClear(RuleAction *action) {
    if (action == 0) return;

    action->setCellularMode(-1);
}

QString
QmlRuleUtil::standByScreenModeSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    if (action == 0) return nonUsable;

    QString summary;
    switch (action->getStandByScreenMode()) {
    case 0:
        summary = tr("Stand-by screen off"); break;
    case 1:
        summary = tr("Stand-by screen on"); break;
    default:
        return nonUsable;
    }

    if (!inListing && action->getRestoreStandByScreenMode()) {
        summary += tr(". Restores previous mode.");
    }

    return summary;
}

void
QmlRuleUtil::standByScreenModeClear(RuleAction *action) {
    if (action == 0) return;

    action->setStandByScreenMode(-1);
    action->setRestoreStandByScreenMode(false);
}

QString
QmlRuleUtil::customActionSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    if (action == 0) return nonUsable;

    QString commandLine = action->getCommandLine().trimmed();
    QString commandLineExit = action->getCommandLineExit().trimmed();
    if (inListing) {
        return (!commandLine.isEmpty() || !commandLineExit.isEmpty() ? tr("Command line") : nonUsable);
    } else {
        if (!commandLine.isEmpty() &&
                !commandLineExit.isEmpty()) {
            return tr("Custom action on activate and deactivate has been set");
        }
        else if (!commandLine.isEmpty()) {
            return tr("Custom action on activate has been set");
        }
        else if (!commandLineExit.isEmpty()) {
            return tr("Custom action on deactivate has been set");
        }
    }
    return nonUsable;
}

void
QmlRuleUtil::customActionClear(RuleAction *action) {
    if (action == 0) return;

    action->setCommandLine(QString());
    action->setCommandLineExit(QString());
}

QString
QmlRuleUtil::applicationSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    Q_UNUSED(inListing)

    if (action == 0) return nonUsable;

    if (action->application().getLaunchers().size() > 0) {
        return tr("Launch applications");
    }
    return nonUsable;
}

void
QmlRuleUtil::applicationClear(RuleAction *action) {
    if (action == 0) return;

    action->application().setLaunchers(QSet<QString>());
}

QString
QmlRuleUtil::deviceVolumeSummary(RuleAction *action, const QString &nonUsable, bool inListing) {
    qDebug("batteryLevelSummary QML version");
    return deviceVolumeSummary(const_cast<const RuleAction*>(action), nonUsable, inListing);
}

QString
QmlRuleUtil::deviceVolumeSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    Q_UNUSED(inListing)

    if (action == 0) return nonUsable;

    if (action->getDeviceVolume() >= 0) {
        return tr("Device volume %1%").arg(action->getDeviceVolume());
    }

    return nonUsable;
}

void
QmlRuleUtil::deviceVolumeClear(RuleAction *action) {
    if (action == 0) return;

    action->setDeviceVolume(-1);   
}

QString
QmlRuleUtil::alarmSummary(RuleAction *action, const QString &nonUsable, bool inListing) {
    qDebug("alarmSummary QML version");
    return alarmSummary(const_cast<const RuleAction *>(action), nonUsable, inListing);
}

QString
QmlRuleUtil::alarmSummary(const RuleAction *action, const QString &nonUsable, bool inListing) {
    if (action == 0) return nonUsable;

    const RuleActionAlarm &a = action->alarm();
    if (!a.isValid()) {
        return nonUsable;
    }

    if (!inListing) {
        return tr("Alarm with title %1 shown in %2 seconds").arg(a.getTitle()).arg(a.getAlarmInSeconds());
    }
    return tr("Alarm");
}

void
QmlRuleUtil::alarmClear(RuleAction *action) {
    if (action == 0) return;

    qDebug() << "QmlRuleUtil::alarmClear";
    action->alarm().clear();
}
