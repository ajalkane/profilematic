#include "ruleactionalarm.h"

RuleActionAlarm::RuleActionAlarm(QObject *parent)
    : QObject(parent), _alarmInSeconds(-1), _snoozeInMinutes(-1)
{
    _init();
}

RuleActionAlarm::RuleActionAlarm(const RuleActionAlarm &actionAlarm, QObject *parent)
    : QObject(parent)
{
    _title = actionAlarm._title;
    _alarmInSeconds = actionAlarm._alarmInSeconds;
    _snoozeInMinutes = actionAlarm._snoozeInMinutes;
    _sound = actionAlarm._sound;

    _init();
}

RuleActionAlarm &
RuleActionAlarm::operator=(const RuleActionAlarm &o)
{
    _title = o._title;
    _alarmInSeconds = o._alarmInSeconds;
    _snoozeInMinutes = o._snoozeInMinutes;
    _sound = o._sound;

    return *this;
}

void
RuleActionAlarm::_init() {
    connect(this, SIGNAL(titleChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(alarmInSecondsChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(snoozeInMinutesChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(soundChanged()), this, SIGNAL(changed()));
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

void
RuleActionAlarm::setSnoozeInMinutes(int snoozeInMinutes) {
    if (snoozeInMinutes != _snoozeInMinutes) {
        _snoozeInMinutes = snoozeInMinutes;
        emit snoozeInMinutesChanged();
    }
}

void
RuleActionAlarm::setSound(const QString &sound) {
    if (sound != _sound) {
        _sound = sound;
        emit soundChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleActionAlarm &rule)
{
    argument.beginStructure();
    argument << rule.getTitle();
    argument << rule.getAlarmInSeconds();
    argument << rule.getSnoozeInMinutes();
    argument << rule.getSound();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleActionAlarm &rule)
{
    QString title;
    int alarmInSeconds;
    int snoozeInMinutes;
    QString sound;

    argument.beginStructure();
    argument >> title;
    argument >> alarmInSeconds;
    argument >> snoozeInMinutes;
    argument >> sound;
    argument.endStructure();

    rule.setTitle(title);
    rule.setAlarmInSeconds(alarmInSeconds);
    rule.setSnoozeInMinutes(snoozeInMinutes);
    rule.setSound(sound);

    return argument;
}
