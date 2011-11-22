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
#include "rule.h"

Rule::Rule(QObject *parent)
    : QObject(parent),  _profileVolume(-1)
{
}

Rule::Rule(const Rule &o)
    : QObject(NULL),
      _ruleId(o._ruleId),
      _ruleName(o._ruleName),
      _timeStart(o._timeStart),
      _days(o._days),
      _profile(o._profile),
      _profileVolume(o._profileVolume)
{
}

Rule::~Rule() {}

Rule &
Rule::operator=(const Rule &o) {
    _ruleId = o._ruleId;
    _ruleName = o._ruleName;
    _timeStart = o._timeStart;
    _days = o._days;
    _profile = o._profile;
    _profileVolume = o._profileVolume;
    return *this;
}

QString
Rule::getRuleId() const {
    return _ruleId;
}

void
Rule::setRuleId(const QString &ruleId) {
    if (_ruleId != ruleId) {
        _ruleId = ruleId;
        emit ruleIdChanged();
    }
}

QString
Rule::getRuleName() const {
    return _ruleName;
}

void
Rule::setRuleName(const QString &ruleName) {
    if (_ruleName != ruleName) {
        _ruleName = ruleName;
        emit ruleNameChanged();
    }
}

QTime
Rule::getTimeStart() const {
    return _timeStart;
}

void
Rule::setTimeStart(const QTime &timeStart) {
    if (_timeStart != timeStart) {
        _timeStart = timeStart;
        emit timeStartChanged();
    }
}

QString
Rule::getTimeStartQml() const {
    QTime time = getTimeStart();
    int hour = time.hour();
    int min  = time.minute();
    QString timeStr;
    if (hour < 10) {
        timeStr += "0";
    }
    timeStr += QString::number(hour);
    timeStr += ":";
    if (min < 10) {
        timeStr += "0";
    }
    timeStr += QString::number(min);
    qDebug("Rule::timeStartQml returning '%s'", qPrintable(timeStr));
    return timeStr;
}

void
Rule::setTimeStartQml(const QString &timeStart) {
    setTimeStart(QTime::fromString(timeStart, "hh:mm"));
}

const QSet<int> &
Rule::getDays() const {
    return _days;
}

void
Rule::setDays(const QSet<int> &days) {
    if (_days != days) {
        _days = days;
        emit daysChanged();
    }
}

QVariantList
Rule::getDaysQml() const {
    QVariantList days;
    for (QSet<int>::const_iterator i = _days.constBegin(); i != _days.constEnd(); ++i) {
        days << QVariant::fromValue(*i);
    }

    return days;

}
void
Rule::setDaysQml(const QVariantList &days) {
    QSet<int> daysSet;
    for (QVariantList::const_iterator i = days.constBegin(); i != days.constEnd(); ++i) {
        daysSet << i->toInt();
    }
    setDays(daysSet);
}

QString
Rule::getProfile() const {
    return _profile;
}

void
Rule::setProfile(const QString &profile) {
    if (_profile != profile) {
        _profile = profile;
        emit profileChanged();
    }
}

int
Rule::getProfileVolume() const {
    return _profileVolume;
}

void
Rule::setProfileVolume(int volume) {
    if (_profileVolume != volume) {
        _profileVolume = volume;
        emit profileVolumeChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const Rule &rule)
{
    argument.beginStructure();
    argument << rule.getRuleId();
    argument << rule.getRuleName();
    argument << rule.getTimeStart();
    argument << rule.getDays();
    argument << rule.getProfile();
    argument << rule.getProfileVolume();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, Rule &rule)
{
    QString   ruleId = rule.getRuleId();
    QString   ruleName = rule.getRuleName();
    QTime     timeStart = rule.getTimeStart();
    QList<int> days = rule.getDays().toList();

    QString profile = rule.getProfile();
    int     profileVolume = rule.getProfileVolume();

    argument.beginStructure();
    argument >> ruleId;
    argument >> ruleName;
    argument >> timeStart;
    argument >> days;
    argument >> profile;
    argument >> profileVolume;
    argument.endStructure();

    rule.setRuleId(ruleId);
    rule.setRuleName(ruleName);
    rule.setTimeStart(timeStart);
    rule.setDays(QSet<int>::fromList(days));
    rule.setProfile(profile);
    rule.setProfileVolume(profileVolume);

    return argument;
}
