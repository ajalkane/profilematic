#include "ruleactionalarm.h"

RuleActionAlarm::RuleActionAlarm(QObject *parent)
    : QObject(parent), _alarmInSeconds(-1)
{
    _init();
}

RuleActionAlarm::RuleActionAlarm(const RuleActionAlarm &actionAlarm, QObject *parent)
    : QObject(parent)
{
    _title = actionAlarm._title;
    _alarmInSeconds = actionAlarm._alarmInSeconds;

    _init();
}

RuleActionAlarm &
RuleActionAlarm::operator=(const RuleActionAlarm &o)
{
    _title = o._title;
    _alarmInSeconds = o._alarmInSeconds;

    return *this;
}

void
RuleActionAlarm::_init() {
    connect(this, SIGNAL(titleChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(alarmInSecondsChanged()), this, SIGNAL(changed()));
}

void
RuleActionAlarm::setTitle(const QString &title) {
    if (title != _title) {
        _title = title;
        emit titleChanged();
    }
}

void
RuleActionAlarm::setAlarmInSeconds(int alarmInSeconds) {
    if (alarmInSeconds != _alarmInSeconds) {
        _alarmInSeconds = alarmInSeconds;
        emit alarmInSecondsChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleActionAlarm &rule)
{
    argument.beginStructure();
    argument << rule.getTitle();
    argument << rule.getAlarmInSeconds();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleActionAlarm &rule)
{
    QString title;
    int alarmInSeconds;

    argument.beginStructure();
    argument >> title;
    argument >> alarmInSeconds;
    argument.endStructure();

    rule.setTitle(title);
    rule.setAlarmInSeconds(alarmInSeconds);

    return argument;
}
