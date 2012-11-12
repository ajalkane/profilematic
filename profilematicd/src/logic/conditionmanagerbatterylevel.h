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
#ifndef CONDITIONMANAGERBATTERYLEVEL_H
#define CONDITIONMANAGERBATTERYLEVEL_H

#include <QList>

#include "conditionmanager.h"

class ConditionManagerBatteryLevel : public ConditionManager {
    Q_OBJECT

    int _latestLevel;
    int _closestLevelDown;
    int _closestLevelUp;
    int _smallestDistanceDown;
    int _smallestDistanceUp;

    void _monitorBatteryLevel(bool monitor);

    inline int _distance(int level) {
        return level - _latestLevel;
    }
    inline int _distanceToLevel(int distance) {
        return _latestLevel + distance;
    }

    inline void _updateSmallestDistance(int distance) {
        if (distance == 0) return;
        int &updateVar = (distance < 0 ? _smallestDistanceDown : _smallestDistanceUp);
        if (qAbs(distance) < qAbs(updateVar)) {
            updateVar = distance;
        }
    }

public:
    ConditionManagerBatteryLevel();

    virtual void startRefresh();
    virtual bool refresh(const Rule::IdType &, const RuleCondition &rule);
    virtual void matchedRule(const RuleCondition &rule);
    virtual void endRefresh();

private slots:
    void batteryLevelChanged(int batteryLevel);

};

#endif // CONDITIONMANAGERBATTERYLEVEL_H
