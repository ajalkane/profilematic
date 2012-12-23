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

#include "conditionmanagerbatterylevel.h"
#include "../platform/platformutil.h"

namespace {
    int batteryLevelUnknown        = -1;
    int batteryLevelMinUnreachable = -1;
    int batteryLevelMaxUnreachable = 101;
    int smallestDistanceDownNotSet = -101;
    int smallestDistanceUpNotSet   = 101;
}

ConditionManagerBatteryLevel::ConditionManagerBatteryLevel(QObject *parent)
    : ConditionManagerCacheable(parent),
      _latestLevel(batteryLevelUnknown)
{
    setObjectName("ConditionManagerBatteryLevel");

    _clearVars();
}

bool
ConditionManagerBatteryLevel::conditionSetForMatching(const RuleCondition &cond) const {
    return _conditionSetForMatching(cond);
}

ConditionManagerCacheable::MatchStatus
ConditionManagerBatteryLevel::match(const Rule::IdType &ruleId, const RuleCondition &cond) {
    Q_UNUSED(ruleId)

    if (!_conditionSetForMatching(cond)) {
        IFDEBUG(qDebug() << "ConditionManagerBatteryLevel::match() options not set or invalid, matchNotSet");
        return MatchNotSet;
    }

    const RuleConditionBatteryLevel &bl = cond.batteryLevel();

    if (_latestLevel == batteryLevelUnknown) {
        _latestLevel = PlatformUtil::instance()->batteryLevel();
    }

    int distanceMin = _distance(bl.getLevelMin());
    int distanceMax = _distance(bl.getLevelMax());

    bool matches = _latestLevel >= bl.getLevelMin() && _latestLevel <= bl.getLevelMax();

    _updateSmallestDistanceFromMin(distanceMin, matches);
    _updateSmallestDistanceFromMax(distanceMax, matches);
    _limitDown.closestLevel = _distanceToLevel(_limitDown.smallestDistance);
    _limitUp.closestLevel = _distanceToLevel(_limitUp.smallestDistance);

    IFDEBUG(qDebug("ConditionManagerBatteryLevel::match() %d, level %d, range %d-%d, smallestDistance %d/%d",
           matches, _latestLevel, bl.getLevelMin(), bl.getLevelMax(), _limitDown.smallestDistance, _limitUp.smallestDistance));

    return matches ? Matched : NotMatched;
}

void
ConditionManagerBatteryLevel::startMonitor() {
    IFDEBUG(qDebug() << "ConditionManagerBatteryLevel::startMonitor");
    connect(PlatformUtil::instance(), SIGNAL(batteryLevelChanged(int)), this, SLOT(batteryLevelChanged(int)), Qt::UniqueConnection);
    PlatformUtil::instance()->monitorBatteryLevel(true);
}

void
ConditionManagerBatteryLevel::stopMonitor() {
    IFDEBUG(qDebug() << "ConditionManagerBatteryLevel::stopMonitor");
    disconnect(PlatformUtil::instance(), SIGNAL(batteryLevelChanged(int)), this, SLOT(batteryLevelChanged(int)));
    PlatformUtil::instance()->monitorBatteryLevel(false);

    _latestLevel = batteryLevelUnknown;
    _clearVars();
}

void
ConditionManagerBatteryLevel::rulesChanged() {
    IFDEBUG(qDebug() << "ConditionManagerBatteryLevel::rulesChanged");

    _clearVars();
}

void
ConditionManagerBatteryLevel::_clearVars() {
    _limitDown.closestLevel     = batteryLevelMinUnreachable;
    _limitDown.smallestDistance = smallestDistanceDownNotSet;
    _limitUp.closestLevel       = batteryLevelMaxUnreachable;
    _limitUp.smallestDistance   = smallestDistanceUpNotSet;
}

void
ConditionManagerBatteryLevel::batteryLevelChanged(int batteryLevel) {
    IFDEBUG(qDebug() << "ConditionManagerBatteryLevel::batteryLevelChanged" << batteryLevel);
    if (batteryLevel != _latestLevel) {
        _latestLevel = batteryLevel;
        if (_latestLevel < _limitDown.closestLevel || _latestLevel > _limitUp.closestLevel) {
            IFDEBUG(qDebug() << "ConditionManagerBatteryLevel::batteryLevelChanged invalidating");
            _clearVars();
            emit matchInvalidated();
        }
    }
}
