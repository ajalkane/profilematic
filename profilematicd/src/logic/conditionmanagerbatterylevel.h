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

#include "conditionmanagercacheable.h"

class ConditionManagerBatteryLevel : public ConditionManagerCacheable {
    Q_OBJECT

    struct Limit {
        int closestLevel;
        int smallestDistance;
    };

    Limit _limitDown;
    Limit _limitUp;

    int _latestLevel;

    void _monitorBatteryLevel(bool monitor);

    void _clearVars();
    inline bool _conditionSetForMatching(const RuleCondition &cond) const { return cond.batteryLevel().isValid(); }

    inline int _distance(int level) {
        return level - _latestLevel;
    }
    inline int _distanceToLevel(int distance) {
        return _latestLevel + distance;
    }

    inline void _updateSmallestDistanceFromMin(int distance, bool isInclusive) {
        if (distance == 0) {
            _limitDown.smallestDistance = 0;
        } else {
            _updateSmallestDistance(distance, isInclusive);
        }
    }

    inline void _updateSmallestDistanceFromMax(int distance, bool isInclusive) {
        if (distance == 0) {
            _limitUp.smallestDistance = 0;
        } else {
            _updateSmallestDistance(distance, isInclusive);
        }
    }

    // Assumes always called with distance != 0
    inline void _updateSmallestDistance(int distance, bool isInclusive) {
        Limit *updateLimit = (distance < 0 ? &_limitDown : &_limitUp);
        int absDistance = qAbs(distance);
//        qDebug("_updateSmallestDistance: d %d, updateVar %d, d/u: %d/%d", distance, updateLimit->smallestDistance, _limitDown.smallestDistance, _limitUp.smallestDistance);
//        qDebug("_updateSmallestDistance: qAbs(distance) %d qAbs(updateVar) %d", absDistance, qAbs(updateLimit->smallestDistance));
        if (absDistance < qAbs(updateLimit->smallestDistance)) {
            updateLimit->smallestDistance = distance;
            if (!isInclusive) {
                updateLimit->smallestDistance += (distance < 0 ? 1 : -1);
//                qDebug("_updateSmallestDistance: adjusted for not inclusive");
            }
//            qDebug("_updateSmallestDistance: set d %d, updateVar %d, d/u: %d/%d", distance, updateLimit->smallestDistance, _limitDown.smallestDistance, _limitUp.smallestDistance);
        }
    }

public:
    ConditionManagerBatteryLevel(QObject *parent = 0);

    virtual bool conditionSetForMatching(const RuleCondition &cond) const;
    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond);

    virtual void startMonitor();
    virtual void stopMonitor();

    virtual void rulesChanged();

private slots:
    void batteryLevelChanged(int batteryLevel);

};

#endif // CONDITIONMANAGERBATTERYLEVEL_H
