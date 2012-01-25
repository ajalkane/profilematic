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
    : QObject(parent),  _restoreProfile(false), _profileVolume(-1), _flightMode(-1)
{
}

Rule::Rule(const Rule &o)
    : QObject(NULL),
      _ruleId(o._ruleId),
      _ruleName(o._ruleName),
      _timeStart(o._timeStart),
      _timeEnd(o._timeEnd),
      _days(o._days),
      _locationCells(o._locationCells),
      _wlan(o._wlan),
      _profile(o._profile),
      _restoreProfile(o._restoreProfile),
      _profileVolume(o._profileVolume),
      _flightMode(o._flightMode)
{
}

Rule
Rule::createDefaultRule() {
    Rule r = Rule();
    r.setRuleId(DEFAULT_RULE_ID);
    r.setRuleName("Default rule");
    return r;
}

Rule::~Rule() {}

Rule &
Rule::operator=(const Rule &o) {
    _ruleId = o._ruleId;
    _ruleName = o._ruleName;
    conditionsFrom(o);
    actionsFrom(o);
    return *this;
}

Rule &
Rule::conditionsFrom(const Rule &o) {
    _timeStart = o._timeStart;
    _timeEnd = o._timeEnd;
    _days = o._days;
    _locationCells = o._locationCells;
    _wlan = o._wlan;
    return *this;
}

Rule &
Rule::actionsFrom(const Rule &o) {
    _profile = o._profile;
    _restoreProfile = o._restoreProfile;
    _profileVolume = o._profileVolume;
    _flightMode = o._flightMode;
    return *this;
}

bool
Rule::isDefaultRule() const {
    return QLatin1String("defaultRule") == _ruleId;
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

QString
Rule::_getTimeQml(const QTime &time) const {
    if (time.isNull()) {
        qDebug("Rule::_getTimeQml is null, returning empty string");
        return "";
    }
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
    qDebug("Rule::_getTimeQml returning '%s'", qPrintable(timeStr));
    return timeStr;
}

QTime
Rule::getTimeStart() const {
    return _timeStart;
}

void
Rule::setTimeStart(const QTime &timeStart) {    
    // Normalize to always have 00 seconds
    QTime useTime = _timeWithSecs(timeStart, 0);
    if (_timeStart != useTime) {
        _timeStart = useTime;
        emit timeStartChanged();
    }
}

QString
Rule::getTimeStartQml() const {
    return _getTimeQml(getTimeStart());
}

void
Rule::setTimeStartQml(const QString &timeStart) {
    if (timeStart.isEmpty()) {
        setTimeStart(QTime());
    } else {
        setTimeStart(QTime::fromString(timeStart, "hh:mm"));
    }
}

QTime
Rule::getTimeEnd() const {
    return _timeEnd;
}

void
Rule::setTimeEnd(const QTime &timeEnd) {
    QTime useTime = _timeWithSecs(timeEnd, 0);
    if (_timeEnd != useTime) {
        _timeEnd = useTime;
        emit timeEndChanged();
    }
}

QString
Rule::getTimeEndQml() const {
    return _getTimeQml(getTimeEnd());
}

void
Rule::setTimeEndQml(const QString &timeEnd) {
    setTimeEnd(QTime::fromString(timeEnd, "hh:mm"));
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

const QSet<int> &
Rule::getLocationCells() const {
    return _locationCells;
}

void
Rule::setLocationCells(const QSet<int> &cells) {
    if (_locationCells != cells) {
        _locationCells = cells;
        emit locationCellsChanged();
    }
}

bool variantIntLessThan(const QVariant &s1, const QVariant &s2)
{
    return s1.toInt() < s2.toInt();
}

QVariantList
Rule::getLocationCellsQml() const {
    QVariantList cells;
    for (QSet<int>::const_iterator i = _locationCells.constBegin(); i != _locationCells.constEnd(); ++i) {
        cells << QVariant::fromValue(*i);
    }
    qSort(cells.begin(), cells.end(), variantIntLessThan);
    return cells;

}
void
Rule::setLocationCellsQml(const QVariantList &cells) {
    QSet<int> cellsSet;
    for (QVariantList::const_iterator i = cells.constBegin(); i != cells.constEnd(); ++i) {
        cellsSet << i->toInt();
    }
    setLocationCells(cellsSet);
}

const QSet<QString> &
Rule::getWlan() const {
    return _wlan;
}

void
Rule::setWlan(const QSet<QString> &wlan) {
    if (_wlan != wlan) {
        _wlan = wlan;
        emit wlanChanged();
    }
}

bool variantQStringLessThan(const QVariant &s1, const QVariant &s2)
{
    return s1.toString() < s2.toString();
}

QVariantList
Rule::getWlanQml() const {
    QVariantList wlan;
    for (QSet<QString>::const_iterator i = _wlan.constBegin(); i != _wlan.constEnd(); ++i) {
        wlan << QVariant::fromValue(*i);
    }
    qSort(wlan.begin(), wlan.end(), variantIntLessThan);
    return wlan;

}
void
Rule::setWlanQml(const QVariantList &wlan) {
    QSet<QString> wlanSet;
    for (QVariantList::const_iterator i = wlan.constBegin(); i != wlan.constEnd(); ++i) {
        wlanSet << i->toString();
    }
    setWlan(wlanSet);
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

bool
Rule::getRestoreProfile() const {
    return _restoreProfile;
}

void
Rule::setRestoreProfile(bool restore) {
    if (_restoreProfile != restore) {
        _restoreProfile = restore;
        emit restoreProfileChanged();
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

int
Rule::getFlightMode() const {
    return _flightMode;
}

void
Rule::setFlightMode(int flightMode) {
    if (_flightMode != flightMode) {
        _flightMode = flightMode;
        emit flightModeChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const Rule &rule)
{
    argument.beginStructure();
    argument << rule.getRuleId();
    argument << rule.getRuleName();
    argument << rule.getTimeStart();
    argument << rule.getTimeEnd();
    argument << rule.getDays();
    argument << rule.getLocationCells();
    argument << rule.getWlan();
    argument << rule.getProfile();
    argument << rule.getRestoreProfile();
    argument << rule.getProfileVolume();
    argument << rule.getFlightMode();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, Rule &rule)
{
    QString   ruleId = rule.getRuleId();
    QString   ruleName = rule.getRuleName();
    QTime     timeStart = rule.getTimeStart();
    QTime     timeEnd = rule.getTimeEnd();
    QList<int> days = rule.getDays().toList();
    QList<int> cells = rule.getLocationCells().toList();
    QList<QString> wlan = rule.getWlan().toList();

    QString profile = rule.getProfile();
    bool    restoreProfile = rule.getRestoreProfile();
    int     profileVolume = rule.getProfileVolume();
    int     flightMode = rule.getFlightMode();

    argument.beginStructure();
    argument >> ruleId;
    argument >> ruleName;
    argument >> timeStart;
    argument >> timeEnd;
    argument >> days;
    argument >> cells;
    argument >> wlan;
    argument >> profile;
    argument >> restoreProfile;
    argument >> profileVolume;
    argument >> flightMode;
    argument.endStructure();

    rule.setRuleId(ruleId);
    rule.setRuleName(ruleName);
    rule.setTimeStart(timeStart);
    rule.setTimeEnd(timeEnd);
    rule.setDays(QSet<int>::fromList(days));
    rule.setLocationCells(QSet<int>::fromList(cells));
    rule.setWlan(QSet<QString>::fromList(wlan));
    rule.setProfile(profile);
    rule.setRestoreProfile(restoreProfile);
    rule.setProfileVolume(profileVolume);
    rule.setFlightMode(flightMode);

    return argument;
}
