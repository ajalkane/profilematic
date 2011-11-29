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

class Rule : public QObject
{
    Q_OBJECT

    // Conditions
    QString   _ruleId;
    QString   _ruleName;
    QTime     _timeStart;
    QSet<int> _days;

    // Actions
    QString _profile;
    int     _profileVolume;

    // IMPROVE: maybe the QML specifics could be in inheriting class, keeping this
    // class "pure" plain Qt object?
    Q_PROPERTY(QString ruleId READ getRuleId NOTIFY ruleIdChanged)
    Q_PROPERTY(QString ruleName READ getRuleName WRITE setRuleName NOTIFY ruleNameChanged)
    Q_PROPERTY(QString timeStart READ getTimeStartQml WRITE setTimeStartQml NOTIFY timeStartChanged)
    Q_PROPERTY(QVariantList days READ getDaysQml WRITE setDaysQml NOTIFY daysChanged)
    Q_PROPERTY(QString profile READ getProfile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(int profileVolume READ getProfileVolume WRITE setProfileVolume NOTIFY profileVolumeChanged)

signals:
    void ruleNameChanged();
    void ruleIdChanged();
    void timeStartChanged();
    void daysChanged();
    void profileChanged();
    void profileVolumeChanged();
public:
    typedef QString IdType;

    Rule(QObject *parent = 0);
    Rule(const Rule &o);
    virtual ~Rule();
    Rule &operator=(const Rule &o);

    QString getRuleId() const;
    void setRuleId(const QString &ruleId);

    QString getRuleName() const;
    void setRuleName(const QString &ruleName);

    QTime getTimeStart() const;
    void setTimeStart(const QTime &timeStart);
    // For QML
    QString getTimeStartQml() const;
    void setTimeStartQml(const QString &timeStart);

    // 0 = monday, 6 = sunday
    const QSet<int> &getDays() const;
    void setDays(const QSet<int> &days);
    // For QML
    QVariantList getDaysQml() const;
    void setDaysQml(const QVariantList &days);

    QString getProfile() const;
    void setProfile(const QString &profile);

    // Returns -1 if the profile volume is not to be set
    int getProfileVolume() const;
    // Use -1 if the profile volume is not to be set, otherwise value between 0 to 100
    void setProfileVolume(int volume);

    inline bool operator==(const Rule &o) const {
        return this->_ruleId    == o._ruleId
            && this->_ruleName  == o._ruleName
            && this->_timeStart == o._timeStart
            && this->_days      == o._days
            && this->_profile   == o._profile
            && this->_profileVolume == o._profileVolume;
    }    


    inline bool isTimeStartRuleUsable() const {
        return getTimeStart().isValid();
    }
    inline bool isDaysRuleUsable() const {
        return !getDays().isEmpty();
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
