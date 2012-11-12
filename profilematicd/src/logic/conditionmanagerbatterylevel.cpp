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
#include <limits>
#include "conditionmanagerbatterylevel.h"
#include "../platform/platformutil.h"
namespace {
    int batteryLevelUnknown = -1;
}

ConditionManagerBatteryLevel::ConditionManagerBatteryLevel()
{
}

void
ConditionManagerBatteryLevel::startRefresh() {
    _closestLevelDown = INT_MIN;
    _closestLevelUp = INT_MAX;
    _smallestDistanceDown = INT_MIN;
    _smallestDistanceUp = INT_MAX;
}

bool
ConditionManagerBatteryLevel::refresh(const Rule::IdType &ruleId, const RuleCondition &rule) {
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

    _updateSmallestDistance(distanceMin);
    _updateSmallestDistance(distanceMax);

    bool matches = _latestLevel >= bl.getLevelMin() && _latestLevel <= bl.getLevelMax();

    qDebug("ConditionManagerBatteryLevel::refresh matches %d, level %d, range %d-%d",
           matches, _latestLevel, bl.getLevelMin(), bl.getLevelMax());

    return matches;
}

void
ConditionManagerBatteryLevel::matchedRule(const RuleCondition &/*rule*/) {
}

void
ConditionManagerBatteryLevel::endRefresh() {
    if (_smallestDistanceDown != INT_MIN) {
        _monitorBatteryLevel(true);
        _closestLevelDown = _distanceToLevel(_smallestDistanceDown);
        _closestLevelUp = _distanceToLevel(_smallestDistanceUp);

        qDebug("ConditionManagerBatteryLevel::endRefresh current level %d, down %d, up %d", _latestLevel, _closestLevelDown, _closestLevelUp);
    } else {
        _monitorBatteryLevel(false);
        _latestLevel = batteryLevelUnknown;
    }
}

void
ConditionManagerBatteryLevel::_monitorBatteryLevel(bool monitor) {
    if (monitor) {
        connect(PlatformUtil::instance(), SIGNAL(batteryLevelChanged(int)), this, SLOT(batteryLevelChanged(int)), Qt::UniqueConnection);
    } else {
        disconnect(PlatformUtil::instance(), SIGNAL(batteryLevelChanged(int)), this, SLOT(batteryLevelChanged(int)));
    }
}

void
ConditionManagerBatteryLevel::batteryLevelChanged(int batteryLevel) {
    qDebug("ConditionManagerBatteryLevel::batteryLevelChanged(%d)", batteryLevel);
    if (batteryLevel != _latestLevel) {
        _latestLevel = batteryLevel;
        if (_latestLevel < _closestLevelDown || _latestLevel > _closestLevelUp) {
            qDebug("ConditionManagerBatteryLevel::batteryLevelChanged requesting refresh");
            emit refreshNeeded();
        }
    }
}
