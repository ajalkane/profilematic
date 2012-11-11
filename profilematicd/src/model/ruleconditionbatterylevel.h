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
#ifndef RULECONDITIONBATTERYLEVEL_H
#define RULECONDITIONBATTERYLEVEL_H

#include <QObject>
#include <QDBusArgument>

class RuleConditionBatteryLevel : public QObject
{
    Q_OBJECT

    int _levelMin;
    int _levelMax;

    Q_PROPERTY(int levelMin READ getLevelMin WRITE setLevelMin NOTIFY levelMinChanged)
    Q_PROPERTY(int levelMax READ getLevelMax WRITE setLevelMax NOTIFY levelMaxChanged)

    void _init();
signals:
    void changed();

    void levelMinChanged();
    void levelMaxChanged();

public:
    explicit RuleConditionBatteryLevel(const RuleConditionBatteryLevel &conditionBL, QObject *parent = 0);
    explicit RuleConditionBatteryLevel(QObject *parent = 0);

    RuleConditionBatteryLevel &operator=(const RuleConditionBatteryLevel &other);

    inline bool getLevelMin() const { return _levelMin; }
    inline bool getLevelMax() const { return _levelMax; }

    void setLevelMin(int level);
    void setLevelMax(int level);

    inline bool operator==(const RuleConditionBatteryLevel &o) const {
        return this->_levelMin == o._levelMin
            && this->_levelMax == o._levelMax;
    }

};

Q_DECLARE_METATYPE(RuleConditionBatteryLevel *)
Q_DECLARE_METATYPE(RuleConditionBatteryLevel)

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const RuleConditionBatteryLevel &obj);
const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, RuleConditionBatteryLevel &obj);

#endif // RULECONDITIONBATTERYLEVEL_H
