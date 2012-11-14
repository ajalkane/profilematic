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
#include "conditionmanagerbatterylevel.h"
#include "../platform/platformutil.h"

namespace {
    int batteryLevelUnknown        = -1;
    int batteryLevelMinUnreachable = -1;
    int batteryLevelMaxUnreachable = 101;
    int smallestDistanceDownNotSet = -101;
    int smallestDistanceUpNotSet   = 101;
}

ConditionManagerBatteryLevel::ConditionManagerBatteryLevel()
    : _latestLevel(batteryLevelUnknown),
      _monitoring(false)
{
    _limitDown.closestLevel     = batteryLevelMinUnreachable;
    _limitDown.smallestDistance = smallestDistanceDownNotSet;
    _limitUp.closestLevel       = batteryLevelMaxUnreachable;
    _limitUp.smallestDistance   = smallestDistanceUpNotSet;
}

void
ConditionManagerBatteryLevel::startRefresh() {
    _limitDown.closestLevel     = batteryLevelMinUnreachable;
    _limitDown.smallestDistance = smallestDistanceDownNotSet;
    _limitUp.closestLevel       = batteryLevelMaxUnreachable;
    _limitUp.smallestDistance   = smallestDistanceUpNotSet;
}

bool
ConditionManagerBatteryLevel::refresh(const Rule::IdType &/*ruleId*/, const RuleCondition &rule) {
    const RuleConditionBatteryLevel &bl = rule.batteryLevel();
    if (!bl.isValid()) {
        qDebug("ConditionManagerBatteryLevel::refresh battery levels not set, matches");
        return true;
    }

    if (_latestLevel == batteryLevelUnknown) {
        _latestLevel = PlatformUtil::instance()->batteryLevel();
    }

    int distanceMin = _distance(bl.getLevelMin());
    int distanceMax = _distance(bl.getLevelMax());

    bool matches = _latestLevel >= bl.getLevelMin() && _latestLevel <= bl.getLevelMax();

    _updateSmallestDistanceFromMin(distanceMin, matches);
    _updateSmallestDistanceFromMax(distanceMax, matches);

    qDebug("ConditionManagerBatteryLevel::refresh matches %d, level %d, range %d-%d, smallestDistance %d/%d",
           matches, _latestLevel, bl.getLevelMin(), bl.getLevelMax(), _limitDown.smallestDistance, _limitUp.smallestDistance);

    return matches;
}

void
ConditionManagerBatteryLevel::matchedRule(const RuleCondition &/*rule*/) {
}

void
ConditionManagerBatteryLevel::endRefresh() {
    if (_limitDown.smallestDistance != smallestDistanceDownNotSet ||
        _limitUp.smallestDistance   != smallestDistanceUpNotSet)
    {
        _monitorBatteryLevel(true);
        _limitDown.closestLevel = _distanceToLevel(_limitDown.smallestDistance);
        _limitUp.closestLevel = _distanceToLevel(_limitUp.smallestDistance);

        qDebug("ConditionManagerBatteryLevel::endRefresh current level %d, down %d, up %d", _latestLevel, _limitDown.closestLevel, _limitUp.closestLevel);
    } else {
        _monitorBatteryLevel(false);
        _latestLevel = batteryLevelUnknown;
    }
}

void
ConditionManagerBatteryLevel::_monitorBatteryLevel(bool monitor) {
    if (monitor != _monitoring) {
        _monitoring = monitor;
        if (monitor) {
            connect(PlatformUtil::instance(), SIGNAL(batteryLevelChanged(int)), this, SLOT(batteryLevelChanged(int)), Qt::UniqueConnection);
        } else {
            disconnect(PlatformUtil::instance(), SIGNAL(batteryLevelChanged(int)), this, SLOT(batteryLevelChanged(int)));
        }
        PlatformUtil::instance()->monitorBatteryLevel(monitor);
    }
}

void
ConditionManagerBatteryLevel::batteryLevelChanged(int batteryLevel) {
    qDebug("ConditionManagerBatteryLevel::batteryLevelChanged(%d)", batteryLevel);
    if (batteryLevel != _latestLevel) {
        _latestLevel = batteryLevel;
        if (_latestLevel < _limitDown.closestLevel || _latestLevel > _limitUp.closestLevel) {
            qDebug("ConditionManagerBatteryLevel::batteryLevelChanged requesting refresh");
            emit refreshNeeded();
        }
    }
}
