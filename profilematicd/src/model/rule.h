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
#ifndef RULE_H
#define RULE_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QTime>
#include <QMetaType>
#include <QDBusArgument>

#include "presencerule.h"

#define DEFAULT_RULE_ID "defaultRule"

// IMPROVE: conditions and actions to separate classes
class Rule : public QObject
{
public:
    enum PresenceChangeType {
        CustomPresenceType,
        AllOnlinePresenceType,
        AllOfflinePresenceType
    };

private:
    Q_OBJECT

    // Conditions
    QString   _ruleId;
    QString   _ruleName;
    QTime     _timeStart;
    QTime     _timeEnd;
    QSet<int> _days;
    QSet<int> _locationCells;
    QSet<QString> _wlan;
    int       _wlanTimeout;

    // Actions
    QString _profile;
    bool    _restoreProfile;
    int     _profileVolume;
    int     _flightMode;
    bool    _restoreFlightMode;
    int     _powerSavingMode;
    bool    _restorePowerSavingMode;
    int     _blueToothMode;
    int     _cellularMode;
    QString _commandLine;

    QList<PresenceRule *> _presenceRules;
    QString _presenceStatusMessage;
    bool    _restorePresence;
    PresenceChangeType _presenceChangeType;

    Q_ENUMS(PresenceChangeType)
    // IMPROVE: maybe the QML specifics could be in inheriting class, keeping this
    // class "pure" plain Qt object?
    Q_PROPERTY(QString ruleId READ getRuleId NOTIFY ruleIdChanged)
    Q_PROPERTY(bool isDefaultRule READ isDefaultRule NOTIFY ruleIdChanged STORED false)
    Q_PROPERTY(QString ruleName READ getRuleName WRITE setRuleName NOTIFY ruleNameChanged)
    Q_PROPERTY(QString timeStart READ getTimeStartQml WRITE setTimeStartQml NOTIFY timeStartChanged)
    Q_PROPERTY(QString timeEnd READ getTimeEndQml WRITE setTimeEndQml NOTIFY timeEndChanged)
    Q_PROPERTY(QVariantList days READ getDaysQml WRITE setDaysQml NOTIFY daysChanged)
    Q_PROPERTY(QVariantList locationCells READ getLocationCellsQml WRITE setLocationCellsQml NOTIFY locationCellsChanged)
    Q_PROPERTY(QVariantList wlan READ getWlanQml WRITE setWlanQml NOTIFY wlanChanged)
    Q_PROPERTY(int wlanTimeout READ getWlanTimeout WRITE setWlanTimeout NOTIFY wlanTimeoutChanged)
    Q_PROPERTY(QString profile READ getProfile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(bool restoreProfile READ getRestoreProfile WRITE setRestoreProfile NOTIFY restoreProfileChanged)
    Q_PROPERTY(int profileVolume READ getProfileVolume WRITE setProfileVolume NOTIFY profileVolumeChanged)
    Q_PROPERTY(int flightMode READ getFlightMode WRITE setFlightMode NOTIFY flightModeChanged)
    Q_PROPERTY(bool restoreFlightMode READ getRestoreFlightMode WRITE setRestoreFlightMode NOTIFY restoreFlightModeChanged)
    Q_PROPERTY(int powerSavingMode READ getPowerSavingMode WRITE setPowerSavingMode NOTIFY powerSavingModeChanged)
    Q_PROPERTY(bool restorePowerSavingMode READ getRestorePowerSavingMode WRITE setRestorePowerSavingMode NOTIFY restorePowerSavingModeChanged)
    Q_PROPERTY(int blueToothMode READ getBlueToothMode WRITE setBlueToothMode NOTIFY blueToothModeChanged)
    Q_PROPERTY(int cellularMode READ getCellularMode WRITE setCellularMode NOTIFY cellularModeChanged)
    Q_PROPERTY(QString commandLine READ getCommandLine WRITE setCommandLine NOTIFY commandLineChanged)

    /**
      * This property gives access to the presence rules associated with this
      * rule.
      *
      * \note The setter copies the individual rule instances and binds them
      *       to the Rule instance.
      */
    Q_PROPERTY(QList<QObject *> presenceRules READ presenceRulesQml NOTIFY presenceRulesChanged)
    /**
      * This property represents the global status message set for all accounts
      * whose presence is changed to online by this rule. Specific online
      * status messages for each account can be specified in the corresponding
      * PresenceRule instance.
      */
    Q_PROPERTY(QString presenceStatusMessage READ getPresenceStatusMessage WRITE setPresenceStatusMessage NOTIFY presenceStatusMessageChanged)
    Q_PROPERTY(bool restorePresence READ getRestorePresence WRITE setRestorePresence NOTIFY restorePresenceChanged)
    Q_PROPERTY(PresenceChangeType presenceChangeType READ getPresenceChangeType WRITE setPresenceChangeType NOTIFY presenceChangeTypeChanged)

    QString _getTimeQml(const QTime &time) const;

    inline QTime _timeWithSecs(const QTime time, int secs) const {
        return time.isNull() ? time : QTime(time.hour(), time.minute(), secs);
    }

signals:
    void ruleNameChanged();
    void ruleIdChanged();
    void timeStartChanged();
    void timeEndChanged();
    void daysChanged();
    void locationCellsChanged();
    void wlanChanged();
    void wlanTimeoutChanged();
    void profileChanged();
    void restoreProfileChanged();
    void profileVolumeChanged();
    void flightModeChanged();
    void restoreFlightModeChanged();
    void powerSavingModeChanged();
    void restorePowerSavingModeChanged();
    void blueToothModeChanged();
    void cellularModeChanged();
    void commandLineChanged();
    void presenceRulesChanged();
    void presenceStatusMessageChanged();
    void restorePresenceChanged();
    void presenceChangeTypeChanged();
private slots:
    void onPresenceRuleActionChanged();
public:
    typedef QString IdType;

    Rule(QObject *parent = 0);
    Rule(const Rule &o);
    static Rule createDefaultRule();

    virtual ~Rule();
    Rule &operator=(const Rule &o);
    Rule &conditionsFrom(const Rule &o);
    Rule &actionsFrom(const Rule &o);
    bool isDefaultRule() const;

    QString getRuleId() const;
    void setRuleId(const QString &ruleId);

    QString getRuleName() const;
    void setRuleName(const QString &ruleName);

    QTime getTimeStart() const;
    void setTimeStart(const QTime &timeStart);
    // For QML
    QString getTimeStartQml() const;
    void setTimeStartQml(const QString &timeEnd);
    QTime getTimeEnd() const;
    void setTimeEnd(const QTime &timeEnd);
    // For QML
    QString getTimeEndQml() const;
    void setTimeEndQml(const QString &timeEnd);
    inline QTime getTimeDuration() const {
        int secsToEnd = _timeStart.secsTo(_timeEnd);
        qDebug("secsToEnd %d", secsToEnd);
        if (secsToEnd <= 0) {
            secsToEnd += 24 * 60 * 60;
        }
        return QTime().addSecs(secsToEnd);
    }
    // 0 = monday, 6 = sunday
    const QSet<int> &getDays() const;
    void setDays(const QSet<int> &days);
    // For QML
    QVariantList getDaysQml() const;
    void setDaysQml(const QVariantList &days);

    const QSet<int> &getLocationCells() const;
    void setLocationCells(const QSet<int> &cells);
    // For QML
    QVariantList getLocationCellsQml() const;
    void setLocationCellsQml(const QVariantList &cells);

    const QSet<QString> &getWlan() const;
    void setWlan(const QSet<QString> &wlan);
    // For QML
    QVariantList getWlanQml() const;
    void setWlanQml(const QVariantList &wlan);

    int getWlanTimeout() const;
    void setWlanTimeout(int timeoutSecs);

    QString getProfile() const;
    void setProfile(const QString &profile);

    bool getRestoreProfile() const;
    void setRestoreProfile(bool restore);

    // Returns -1 if the profile volume is not to be set
    int getProfileVolume() const;
    // Use -1 if the profile volume is not to be set, otherwise value between 0 to 100
    void setProfileVolume(int volume);

    // -1 don't set, 0, set no flight mode, 1 set flight mode
    int getFlightMode() const;
    void setFlightMode(int mode);
    bool getRestoreFlightMode() const;
    void setRestoreFlightMode(bool restore);

    // -1 don't set, 0, set no power saving mode, 1 set power saving mode
    int getPowerSavingMode() const;
    void setPowerSavingMode(int state);
    bool getRestorePowerSavingMode() const;
    void setRestorePowerSavingMode(bool restore);

    // -1 don't set, 0 set BlueTooth off, 1 set BlueTooth on, 2 set BlueTooth on and visible
    int getBlueToothMode() const;
    void setBlueToothMode(int mode);

    // -1 don't set, 0 set Dual, 1 set 2G, 2 set 3G
    int getCellularMode() const;
    void setCellularMode(int mode);

    QString getCommandLine() const;
    void setCommandLine(const QString &commandLine);

    QList<QObject *> presenceRulesQml() const;

    QList<PresenceRule *> presenceRules() const;
    void setPresenceRules(QList<PresenceRule *> presenceRules);

    bool addPresenceRule(PresenceRule *presenceRule);
    bool removePresenceRule(PresenceRule *presenceRule);
    PresenceRule *presenceRule(const PresenceRule::Key &key);

    const QString &getPresenceStatusMessage() const;
    void setPresenceStatusMessage(const QString &pressenceStatusMessage);
    void updatePresenceChangeType(const PresenceRule *presenceRule);

    bool getRestorePresence() const;
    void setRestorePresence(bool restorePresence);

    PresenceChangeType getPresenceChangeType() const;
    void setPresenceChangeType(PresenceChangeType presenceChangeType);

    inline bool operator==(const Rule &o) const {
        return this->_ruleId    == o._ruleId
            && this->_ruleName  == o._ruleName
            && this->_timeStart == o._timeStart
            && this->_timeEnd   == o._timeEnd
            && this->_days      == o._days
            && this->_locationCells == o._locationCells
            && this->_wlan      == o._wlan
            && this->_profile   == o._profile
            && this->_restoreProfile == o._restoreProfile
            && this->_profileVolume == o._profileVolume
            && this->_flightMode == o._flightMode
            && this->_restoreFlightMode == o._restoreFlightMode
            && this->_powerSavingMode == o._powerSavingMode
            && this->_restorePowerSavingMode == o._restorePowerSavingMode
            && this->_blueToothMode == o._blueToothMode
            && this->_cellularMode == o._cellularMode
            && this->_commandLine == o._commandLine
            && this->_presenceStatusMessage == o._presenceStatusMessage
            && this->_restorePresence == o._restorePresence;
    }


    inline bool isTimeStartRuleUsable() const {
        return getTimeStart().isValid();
    }
    inline bool isTimeEndRuleUsable() const {
        return getTimeEnd().isValid();
    }
    inline bool isDaysRuleUsable() const {
        return !getDays().isEmpty();
    }
    inline bool isLocationCellsRuleUsable() const {
        return !getLocationCells().isEmpty();
    }
};

Q_DECLARE_METATYPE(Rule)
// TODO this needs cleaning up
Q_DECLARE_METATYPE(QList<Rule>)

// Marshall the Rule data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const Rule &mystruct);

// Retrieve the Rule data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, Rule &mystruct);

#endif // RULE_H
