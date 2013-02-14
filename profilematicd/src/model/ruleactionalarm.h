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
#ifndef RULEACTIONALARM_H
#define RULEACTIONALARM_H

#include <QObject>
#include <QString>
#include <QDBusArgument>

class RuleActionAlarm : public QObject
{
    Q_OBJECT

    QString _title;
    int _alarmInSeconds;

    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int alarmInSeconds READ getAlarmInSeconds WRITE setAlarmInSeconds NOTIFY alarmInSecondsChanged)

    void _init();
signals:
    void changed();

    void titleChanged();
    void alarmInSecondsChanged();

public:
    explicit RuleActionAlarm(QObject *parent = 0);
    explicit RuleActionAlarm(const RuleActionAlarm &actionAlarm, QObject *parent = 0);

    RuleActionAlarm &operator=(const RuleActionAlarm &other);

    inline const QString &getTitle() const { return _title; }
    void setTitle(const QString &title);

    inline const int &getAlarmInSeconds() const { return _alarmInSeconds; }
    void setAlarmInSeconds(int alarmInSeconds);

    inline bool isValid() const {
        return !_title.isEmpty() && _alarmInSeconds >= 0;
    }

    inline void clear() {
        setTitle("");
        setAlarmInSeconds(-1);
    }

    inline bool operator==(const RuleActionAlarm &o) const {
        return this->_title == o._title
            && this->_alarmInSeconds == o._alarmInSeconds;
    }

};

Q_DECLARE_METATYPE(RuleActionAlarm *)
Q_DECLARE_METATYPE(RuleActionAlarm)

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const RuleActionAlarm &obj);
const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, RuleActionAlarm &obj);

#endif // RULEACTIONALARM_H
