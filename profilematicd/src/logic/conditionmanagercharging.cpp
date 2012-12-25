/**********************************************************************
 * Copyright 2012 Arto Jalkanen
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

#include "conditionmanagercharging.h"

ConditionManagerCharging::ConditionManagerCharging(QObject *parent)
    : ConditionManagerCacheable(parent), _currentChargingState(RuleCondition::UndefinedChargingState)
{
    setObjectName("ConditionManagerCharging");
}

bool
ConditionManagerCharging::conditionSetForMatching(const RuleCondition &cond) const {
    return _conditionSetForMatching(cond);
}

ConditionManagerCacheable::MatchStatus
ConditionManagerCharging::match(const Rule::IdType &ruleId, const RuleCondition &cond) {
    Q_UNUSED(ruleId)

    if (!_conditionSetForMatching(cond)) {
        IFDEBUG(qDebug() << "ConditionManagerCharging::match() options not set or invalid, matchNotSet");
        return MatchNotSet;
    }

    if (_currentChargingState == RuleCondition::UndefinedChargingState) {
        _currentChargingState = (RuleCondition::ChargingState)PlatformUtil::instance()->batteryChargingState();
    }

    RuleCondition::ChargingState chargingState = cond.getChargingState();

    IFDEBUG(qDebug("ConditionManagerCharging::refresh current %d rule %d", _currentChargingState, chargingState));
    return _currentChargingState == chargingState ? Matched : NotMatched;
}

void
ConditionManagerCharging::startMonitor() {
    IFDEBUG(qDebug() << "ConditionManagerCharging::startMonitor");
    connect(PlatformUtil::instance(), SIGNAL(batteryChargingStateChanged(int)), this, SLOT(batteryChargingStateChanged(int)));
}

void
ConditionManagerCharging::stopMonitor() {
    IFDEBUG(qDebug() << "ConditionManagerCharging::stopMonitor");
    disconnect(PlatformUtil::instance(), SIGNAL(batteryChargingStateChanged(int)), this, SLOT(batteryChargingStateChanged(int)));

    _clearVars();
}

void
ConditionManagerCharging::rulesChanged() {
    IFDEBUG(qDebug() << "ConditionManagerCharging::rulesChanged");
}

void
ConditionManagerCharging::batteryChargingStateChanged(int chargingState) {
    IFDEBUG(qDebug() << QDateTime::currentDateTime().toString()
                     << "ConditionManagerCharging::batteryChargingStateChanged"
                     << chargingState
                     << "current charging state"
                     << _currentChargingState
                     << ", invalidating");

    _currentChargingState = (RuleCondition::ChargingState)chargingState;

    emit matchInvalidated();
}

void
ConditionManagerCharging::_clearVars() {
    _currentChargingState = RuleCondition::UndefinedChargingState;
}

