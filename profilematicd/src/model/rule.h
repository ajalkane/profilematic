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

#include "ruleaction.h"

#define DEFAULT_RULE_ID "defaultRule"

// IMPROVE: conditions to separate class
// IMPROVE: it'd be cleaner to have RuleAction as member of Rule, but inheriting from it
// will be less painful transition
class Rule : public RuleAction
{
    Q_OBJECT

    QString   _ruleId;
    QString   _ruleName;
    // Conditions
    QTime     _timeStart;
    QTime     _timeEnd;
    QSet<int> _days;
    QSet<int> _locationCells;
    QSet<QString> _wlan;
    int       _wlanTimeout;

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
public:
    typedef QString IdType;

    Rule(QObject *parent = 0);
    Rule(const Rule &o);
    static Rule createDefaultRule();

    virtual ~Rule();
    Rule &operator=(const Rule &o);
    Rule &conditionsFrom(const Rule &o);
    Rule &actionsFrom(const RuleAction &o);
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

    inline bool operator==(const Rule &o) const {
        return this->_ruleId    == o._ruleId
            && this->_ruleName  == o._ruleName
            && this->_timeStart == o._timeStart
            && this->_timeEnd   == o._timeEnd
            && this->_days      == o._days
            && this->_locationCells == o._locationCells
            && this->_wlan      == o._wlan
            && RuleAction::operator==(o);
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
