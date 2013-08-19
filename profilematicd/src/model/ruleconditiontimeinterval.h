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
#ifndef RULECONDITIONTIMEINTERVAL_H
#define RULECONDITIONTIMEINTERVAL_H

#include <QObject>
#include <QDBusArgument>

class RuleConditionTimeInterval : public QObject
{
    Q_OBJECT

    int _activeForSecs;
    int _inactiveForSecs;

    Q_PROPERTY(int inactiveForSecs READ getInactiveForSecs WRITE setInactiveForSecs NOTIFY inactiveForSecsChanged)
    Q_PROPERTY(int activeForSecs READ getActiveForSecs WRITE setActiveForSecs NOTIFY activeForSecsChanged)

    void _init();
signals:
    void changed();

    void inactiveForSecsChanged();
    void activeForSecsChanged();

public:
    explicit RuleConditionTimeInterval(const RuleConditionTimeInterval &conditionBL, QObject *parent = 0);
    explicit RuleConditionTimeInterval(QObject *parent = 0);

    RuleConditionTimeInterval &operator=(const RuleConditionTimeInterval &other);

    inline int getActiveForSecs() const { return _activeForSecs; }
    inline int getInactiveForSecs() const { return _inactiveForSecs; }

    // Both must be set, and at least one of them must be non-zero or a busy loop is possible
    Q_INVOKABLE inline bool isValid() const { return (getActiveForSecs() > -1 && getInactiveForSecs() > -1) && (getActiveForSecs() > 0 || getInactiveForSecs() > 0); }

    void setActiveForSecs(int secs);
    void setInactiveForSecs(int secs);

    Q_INVOKABLE inline void clear() {
        setActiveForSecs(-1);
        setInactiveForSecs(-1);
    }

    inline bool operator==(const RuleConditionTimeInterval &o) const {
        return this->_activeForSecs == o._activeForSecs
            && this->_inactiveForSecs == o._inactiveForSecs;
    }

};

Q_DECLARE_METATYPE(RuleConditionTimeInterval *)
Q_DECLARE_METATYPE(RuleConditionTimeInterval)

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const RuleConditionTimeInterval &obj);
const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, RuleConditionTimeInterval &obj);

#endif // RULECONDITIONTIMEINTERVAL_H
