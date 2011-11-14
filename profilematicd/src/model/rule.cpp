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

Rule::Rule()
    : _ruleId(0), _profileVolume(-1)
{
}

int
Rule::getRuleId() const {
    return _ruleId;
}

void
Rule::setRuleId(int ruleId) {
    _ruleId = ruleId;
}

QString
Rule::getRuleName() const {
    return _ruleName;
}

void
Rule::setRuleName(const QString &ruleName) {
    _ruleName = ruleName;
}

QTime
Rule::getTimeStart() const {
    return _timeStart;
}

void
Rule::setTimeStart(const QTime &timeStart) {
    _timeStart = timeStart;
}

const QSet<int> &
Rule::getDays() const {
    return _days;
}

void
Rule::setDays(const QSet<int> &days) {
    _days = days;
}

QString
Rule::getProfile() const {
    return _profile;
}

void
Rule::setProfile(const QString &profile) {
    _profile = profile;
}

int
Rule::getProfileVolume() const {
    return _profileVolume;
}

void
Rule::setProfileVolume(int volume) {
    _profileVolume = volume;
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
    int       ruleId = rule.getRuleId();
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
