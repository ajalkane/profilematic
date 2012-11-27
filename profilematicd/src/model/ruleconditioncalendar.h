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
#ifndef RULECONDITIONCALENDAR_H
#define RULECONDITIONCALENDAR_H

#include <QObject>
#include <QString>
#include <QDBusArgument>

class RuleConditionCalendar : public QObject
{
    Q_OBJECT

    QString _summaryMatch;

    Q_PROPERTY(QString summaryMatch READ getSummaryMatch WRITE setSummaryMatch NOTIFY summaryMatchChanged)

    void _init();
signals:
    void changed();

    void summaryMatchChanged();

public:
public:
    explicit RuleConditionCalendar(const RuleConditionCalendar &conditionCal, QObject *parent = 0);
    explicit RuleConditionCalendar(QObject *parent = 0);

    RuleConditionCalendar &operator=(const RuleConditionCalendar &other);

    inline QString getSummaryMatch() const { return _summaryMatch; }
    void setSummaryMatch(const QString &match);

    Q_INVOKABLE inline bool isValid() const { return !_summaryMatch.isEmpty(); }

    Q_INVOKABLE inline void clear() {
        setSummaryMatch(QString());
    }

    inline bool operator==(const RuleConditionCalendar &o) const {
        return this->_summaryMatch == o._summaryMatch;
    }
};

Q_DECLARE_METATYPE(RuleConditionCalendar *)
Q_DECLARE_METATYPE(RuleConditionCalendar)

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const RuleConditionCalendar &obj);
const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, RuleConditionCalendar &obj);

#endif // RULECONDITIONCALENDAR_H
