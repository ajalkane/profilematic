/**********************************************************************
 * Copyright 2013 Arto Jalkanen
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

#include "ruleconditiontimeinterval.h"

RuleConditionTimeInterval::RuleConditionTimeInterval(QObject *parent)
    : QObject(parent), _activeForSecs(-1), _inactiveForSecs(-1)
{
    _init();
}

RuleConditionTimeInterval::RuleConditionTimeInterval(const RuleConditionTimeInterval &o, QObject *parent)
    : QObject(parent)
{
    _activeForSecs = o._activeForSecs;
    _inactiveForSecs = o._inactiveForSecs;

    _init();
}

RuleConditionTimeInterval &
RuleConditionTimeInterval::operator=(const RuleConditionTimeInterval &o)
{
    _activeForSecs = o._activeForSecs;
    _inactiveForSecs = o._inactiveForSecs;

    return *this;
}

void
RuleConditionTimeInterval::_init() {
    connect(this, SIGNAL(activeForSecsChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(inactiveForSecsChanged()), this, SIGNAL(changed()));
}

void
RuleConditionTimeInterval::setActiveForSecs(int secs) {
    if (secs != _activeForSecs) {
        _activeForSecs = secs;
        emit activeForSecsChanged();
    }
}

void
RuleConditionTimeInterval::setInactiveForSecs(int secs) {
    if (secs != _inactiveForSecs) {
        _inactiveForSecs = secs;
        emit inactiveForSecsChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleConditionTimeInterval &rule)
{
    argument.beginStructure();
    argument << rule.getActiveForSecs();
    argument << rule.getInactiveForSecs();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleConditionTimeInterval &rule)
{
    int activeForSecs;
    int inactiveForSecs;

    argument.beginStructure();
    argument >> activeForSecs;
    argument >> inactiveForSecs;
    argument.endStructure();

    rule.setActiveForSecs(activeForSecs);
    rule.setInactiveForSecs(inactiveForSecs);

    return argument;
}
