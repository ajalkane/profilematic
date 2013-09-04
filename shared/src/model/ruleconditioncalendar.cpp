#include "ruleconditioncalendar.h"

RuleConditionCalendar::RuleConditionCalendar(QObject *parent)
    : QObject(parent), _timePrepend(0), _timeAppend(0)
{
    _init();
}

RuleConditionCalendar::RuleConditionCalendar(const RuleConditionCalendar &o, QObject *parent)
    : QObject(parent)
{
    _summaryMatch = o._summaryMatch;
    _locationMatch = o._locationMatch;
    _timePrepend = o._timePrepend;
    _timeAppend = o._timeAppend;

    _init();
}

RuleConditionCalendar &
RuleConditionCalendar::operator=(const RuleConditionCalendar &o)
{
    _summaryMatch = o._summaryMatch;
    _locationMatch = o._locationMatch;
    _timePrepend = o._timePrepend;
    _timeAppend = o._timeAppend;

    return *this;
}

void
RuleConditionCalendar::_init() {
    connect(this, SIGNAL(summaryMatchChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(locationMatchChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(timePrependChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(timeAppendChanged()), this, SIGNAL(changed()));
}

void
RuleConditionCalendar::setSummaryMatch(const QString &match) {
    if (match != _summaryMatch) {
        _summaryMatch = match;
        emit summaryMatchChanged();
    }
}

void
RuleConditionCalendar::setLocationMatch(const QString &match) {
    if (match != _locationMatch) {
        _locationMatch = match;
        emit locationMatchChanged();
    }
}

void
RuleConditionCalendar::setTimePrepend(int secs) {
    if (secs != _timePrepend) {
        _timePrepend = secs;
        emit timePrependChanged();
    }
}

void
RuleConditionCalendar::setTimeAppend(int secs) {
    if (secs != _timeAppend) {
        _timeAppend = secs;
        emit timeAppendChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleConditionCalendar &rule)
{
    argument.beginStructure();
    argument << rule.getSummaryMatch();
    argument << rule.getLocationMatch();
    argument << rule.getTimePrepend();
    argument << rule.getTimeAppend();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleConditionCalendar &rule)
{
    QString summaryMatch;
    QString locationMatch;
    int timePrepend;
    int timeAppend;

    argument.beginStructure();
    argument >> summaryMatch;
    argument >> locationMatch;
    argument >> timePrepend;
    argument >> timeAppend;
    argument.endStructure();

    rule.setSummaryMatch(summaryMatch);
    rule.setLocationMatch(locationMatch);
    rule.setTimePrepend(timePrepend);
    rule.setTimeAppend(timeAppend);

    return argument;
}
