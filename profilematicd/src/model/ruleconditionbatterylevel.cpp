#include "ruleconditionbatterylevel.h"

RuleConditionBatteryLevel::RuleConditionBatteryLevel(QObject *parent)
    : QObject(parent), _levelMin(-1), _levelMax(-1)
{
    _init();
}

RuleConditionBatteryLevel::RuleConditionBatteryLevel(const RuleConditionBatteryLevel &o, QObject *parent)
    : QObject(parent)
{
    _levelMin = o._levelMin;
    _levelMax = o._levelMax;

    _init();
}

RuleConditionBatteryLevel &
RuleConditionBatteryLevel::operator=(const RuleConditionBatteryLevel &o)
{
    _levelMin = o._levelMin;
    _levelMax = o._levelMax;

    return *this;
}

void
RuleConditionBatteryLevel::_init() {
    connect(this, SIGNAL(levelMinChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(levelMaxChanged()), this, SIGNAL(changed()));
}

void
RuleConditionBatteryLevel::setLevelMin(int level) {
    if (level != _levelMin) {
        _levelMin = level;
        emit levelMinChanged();
    }
}

void
RuleConditionBatteryLevel::setLevelMax(int level) {
    if (level != _levelMax) {
        _levelMax = level;
        emit levelMaxChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleConditionBatteryLevel &rule)
{
    argument.beginStructure();
    argument << rule.getLevelMin();
    argument << rule.getLevelMax();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleConditionBatteryLevel &rule)
{
    int levelMin;
    int levelMax;

    argument.beginStructure();
    argument >> levelMin;
    argument >> levelMax;
    argument.endStructure();

    rule.setLevelMin(levelMin);
    rule.setLevelMax(levelMax);

    return argument;
}
