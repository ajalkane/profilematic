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
    QString _locationMatch;
    int _timePrepend;
    int _timeAppend;

    Q_PROPERTY(QString summaryMatch READ getSummaryMatch WRITE setSummaryMatch NOTIFY summaryMatchChanged)
    Q_PROPERTY(QString locationMatch READ getLocationMatch WRITE setLocationMatch NOTIFY summaryMatchChanged)
    Q_PROPERTY(int timePrepend READ getTimePrepend WRITE setTimePrepend NOTIFY timePrependChanged)
    Q_PROPERTY(int timeAppend READ getTimeAppend WRITE setTimeAppend NOTIFY timeAppendChanged)

    void _init();
signals:
    void changed();

    void summaryMatchChanged();
    void locationMatchChanged();
    void timePrependChanged();
    void timeAppendChanged();

public:
public:
    explicit RuleConditionCalendar(const RuleConditionCalendar &conditionCal, QObject *parent = 0);
    explicit RuleConditionCalendar(QObject *parent = 0);

    RuleConditionCalendar &operator=(const RuleConditionCalendar &other);

    inline QString getSummaryMatch() const { return _summaryMatch; }
    void setSummaryMatch(const QString &match);

    inline QString getLocationMatch() const { return _locationMatch; }
    void setLocationMatch(const QString &match);

    inline int getTimePrepend() const { return _timePrepend; }
    void setTimePrepend(int secs);

    inline int getTimeAppend() const { return _timeAppend; }
    void setTimeAppend(int secs);

    Q_INVOKABLE inline bool isValid() const { return !(_summaryMatch.isEmpty() && _locationMatch.isEmpty()); }

    Q_INVOKABLE inline void clear() {
        setSummaryMatch(QString());
        setLocationMatch(QString());
    }

    inline bool operator==(const RuleConditionCalendar &o) const {
        return this->_summaryMatch == o._summaryMatch
            && this->_locationMatch == o._locationMatch
            && this->_timePrepend == o._timePrepend
            && this->_timeAppend == o._timeAppend;
    }
};

Q_DECLARE_METATYPE(RuleConditionCalendar *)
Q_DECLARE_METATYPE(RuleConditionCalendar)

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const RuleConditionCalendar &obj);
const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, RuleConditionCalendar &obj);

#endif // RULECONDITIONCALENDAR_H
