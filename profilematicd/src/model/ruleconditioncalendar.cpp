#include "ruleconditioncalendar.h"

RuleConditionCalendar::RuleConditionCalendar(QObject *parent)
    : QObject(parent)
{
    _init();
}

RuleConditionCalendar::RuleConditionCalendar(const RuleConditionCalendar &o, QObject *parent)
    : QObject(parent)
{
    _summaryMatch = o._summaryMatch;

    _init();
}

RuleConditionCalendar &
RuleConditionCalendar::operator=(const RuleConditionCalendar &o)
{
    _summaryMatch = o._summaryMatch;

    return *this;
}

void
RuleConditionCalendar::_init() {
    connect(this, SIGNAL(summaryMatchChanged()), this, SIGNAL(changed()));
}

void
RuleConditionCalendar::setSummaryMatch(const QString &match) {
    if (match != _summaryMatch) {
        _summaryMatch = match;
        emit summaryMatchChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleConditionCalendar &rule)
{
    argument.beginStructure();
    argument << rule.getSummaryMatch();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleConditionCalendar &rule)
{
    QString summaryMatch;

    argument.beginStructure();
    argument >> summaryMatch;
    argument.endStructure();

    rule.setSummaryMatch(summaryMatch);

    return argument;
}
