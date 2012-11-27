#include "rulecondition.h"

RuleCondition::RuleCondition(QObject *parent)
    : QObject(parent), _locationCellsTimeout(0), _wlanTimeout(0), _idleForSecs(-1),
      _internetConnectionMode(UndefinedInternetConnectionMode),
      _chargingState(UndefinedChargingState)

{
    _init();
}

RuleCondition::RuleCondition(const RuleCondition &o)
    : QObject(NULL),
      _timeStart(o._timeStart),
      _timeEnd(o._timeEnd),
      _days(o._days),
      _locationCells(o._locationCells),
      _locationCellsTimeout(o._locationCellsTimeout),
      _wlan(o._wlan),
      _wlanTimeout(o._wlanTimeout),
      _idleForSecs(o._idleForSecs),
      _nfc(o._nfc),
      _internetConnectionMode(o._internetConnectionMode),
      _chargingState(o._chargingState),
      _batteryLevel(o._batteryLevel),
      _calendar(o._calendar)
{
    _init();
}

void
RuleCondition::_init() {
    connect(this, SIGNAL(timeStartChanged()),     this, SIGNAL(changed()));
    connect(this, SIGNAL(timeEndChanged()),       this, SIGNAL(changed()));
    connect(this, SIGNAL(daysChanged()),          this, SIGNAL(changed()));
    connect(this, SIGNAL(locationCellsChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(locationCellsTimeoutChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(wlanChanged()),          this, SIGNAL(changed()));
    connect(this, SIGNAL(wlanTimeoutChanged()),   this, SIGNAL(changed()));
    connect(this, SIGNAL(idleForSecsChanged()),   this, SIGNAL(changed()));

    connect(&_nfc, SIGNAL(changed()),   this, SIGNAL(nfcChanged()));
    connect(this, SIGNAL(nfcChanged()),   this, SIGNAL(changed()));
    connect(this, SIGNAL(internetConnectionModeChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(chargingStateChanged()), this, SIGNAL(changed()));
    connect(&_batteryLevel, SIGNAL(changed()),   this, SIGNAL(batteryLevelChanged()));
    connect(this, SIGNAL(batteryLevelChanged()), this, SIGNAL(changed()));
    connect(&_calendar,     SIGNAL(changed()),   this, SIGNAL(calendarChanged()));
    connect(this, SIGNAL(calendarChanged()), this, SIGNAL(changed()));
}


RuleCondition &
RuleCondition::operator=(const RuleCondition &o)
{
    _timeStart = o._timeStart;
    _timeEnd = o._timeEnd;
    _days = o._days;
    _locationCells = o._locationCells;
    _locationCellsTimeout = o._locationCellsTimeout;
    _wlan = o._wlan;
    _wlanTimeout = o._wlanTimeout;
    _idleForSecs = o._idleForSecs;
    _nfc = o._nfc;
    _internetConnectionMode = o._internetConnectionMode;
    _chargingState = o._chargingState;
    _batteryLevel = o._batteryLevel;
    _calendar = o._calendar;

    return *this;
}

RuleCondition::~RuleCondition() {}

QString
RuleCondition::_getTimeQml(const QTime &time) const {
    if (time.isNull()) {
        return "";
    }
    int hour = time.hour();
    int min  = time.minute();
    QString timeStr;
    if (hour < 10) {
        timeStr += "0";
    }
    timeStr += QString::number(hour);
    timeStr += ":";
    if (min < 10) {
        timeStr += "0";
    }
    timeStr += QString::number(min);
    return timeStr;
}

QTime
RuleCondition::getTimeStart() const {
    return _timeStart;
}

void
RuleCondition::setTimeStart(const QTime &timeStart) {
    // Normalize to always have 00 seconds
    QTime useTime = _timeWithSecs(timeStart, 0);
    if (_timeStart != useTime) {
        _timeStart = useTime;
        emit timeStartChanged();
    }
}

QString
RuleCondition::getTimeStartQml() const {
    return _getTimeQml(getTimeStart());
}

void
RuleCondition::setTimeStartQml(const QString &timeStart) {
    if (timeStart.isEmpty()) {
        setTimeStart(QTime());
    } else {
        setTimeStart(QTime::fromString(timeStart, "hh:mm"));
    }
}

QTime
RuleCondition::getTimeEnd() const {
    return _timeEnd;
}

void
RuleCondition::setTimeEnd(const QTime &timeEnd) {
    QTime useTime = _timeWithSecs(timeEnd, 0);
    if (_timeEnd != useTime) {
        _timeEnd = useTime;
        emit timeEndChanged();
    }
}

QString
RuleCondition::getTimeEndQml() const {
    return _getTimeQml(getTimeEnd());
}

void
RuleCondition::setTimeEndQml(const QString &timeEnd) {
    setTimeEnd(QTime::fromString(timeEnd, "hh:mm"));
}

const QSet<int> &
RuleCondition::getDays() const {
    return _days;
}

void
RuleCondition::setDays(const QSet<int> &days) {
    if (_days != days) {
        _days = days;
        emit daysChanged();
    }
}

QVariantList
RuleCondition::getDaysQml() const {
    QVariantList days;
    for (QSet<int>::const_iterator i = _days.constBegin(); i != _days.constEnd(); ++i) {
        days << QVariant::fromValue(*i);
    }

    return days;

}
void
RuleCondition::setDaysQml(const QVariantList &days) {
    QSet<int> daysSet;
    for (QVariantList::const_iterator i = days.constBegin(); i != days.constEnd(); ++i) {
        daysSet << i->toInt();
    }
    setDays(daysSet);
}

const QSet<int> &
RuleCondition::getLocationCells() const {
    return _locationCells;
}

void
RuleCondition::setLocationCells(const QSet<int> &cells) {
    qDebug("RuleCondition::setLocationCells");
    if (_locationCells != cells) {
        _locationCells = cells;
        qDebug("RuleCondition::setLocationCells emit");
        emit locationCellsChanged();
    }
}

int
RuleCondition::getLocationCellsTimeout() const {
    return _locationCellsTimeout;
}

void
RuleCondition::setLocationCellsTimeout(int timeoutSecs) {
    if (_locationCellsTimeout != timeoutSecs) {
        _locationCellsTimeout = timeoutSecs;
        emit locationCellsTimeoutChanged();
    }
}

bool variantIntLessThan(const QVariant &s1, const QVariant &s2)
{
    return s1.toInt() < s2.toInt();
}

QVariantList
RuleCondition::getLocationCellsQml() const {
    QVariantList cells;
    for (QSet<int>::const_iterator i = _locationCells.constBegin(); i != _locationCells.constEnd(); ++i) {
        cells << QVariant::fromValue(*i);
    }
    qSort(cells.begin(), cells.end(), variantIntLessThan);
    return cells;

}
void
RuleCondition::setLocationCellsQml(const QVariantList &cells) {
    QSet<int> cellsSet;
    for (QVariantList::const_iterator i = cells.constBegin(); i != cells.constEnd(); ++i) {
        cellsSet << i->toInt();
    }
    setLocationCells(cellsSet);
}

const QSet<QString> &
RuleCondition::getWlan() const {
    return _wlan;
}

void
RuleCondition::setWlan(const QSet<QString> &wlan) {
    if (_wlan != wlan) {
        _wlan = wlan;
        emit wlanChanged();
    }
}

// IMPROVE: same function exists in ruleconditionnfc.cpp
namespace {
bool variantQStringLessThan(const QVariant &s1, const QVariant &s2)
{
    return s1.toString() < s2.toString();
}
}

QVariantList
RuleCondition::getWlanQml() const {
    QVariantList wlan;
    for (QSet<QString>::const_iterator i = _wlan.constBegin(); i != _wlan.constEnd(); ++i) {
        wlan << QVariant::fromValue(*i);
    }
    qSort(wlan.begin(), wlan.end(), variantIntLessThan);
    return wlan;

}
void
RuleCondition::setWlanQml(const QVariantList &wlan) {
    QSet<QString> wlanSet;
    for (QVariantList::const_iterator i = wlan.constBegin(); i != wlan.constEnd(); ++i) {
        wlanSet << i->toString();
    }
    setWlan(wlanSet);
}

int
RuleCondition::getWlanTimeout() const {
    return _wlanTimeout;
}

void
RuleCondition::setWlanTimeout(int timeoutSecs) {
    if (_wlanTimeout != timeoutSecs) {
        _wlanTimeout = timeoutSecs;
        emit wlanTimeoutChanged();
    }
}

int
RuleCondition::getIdleForSecs() const
{
    return _idleForSecs;
}

void
RuleCondition::setIdleForSecs(int idleForSecs)
{
    if (_idleForSecs != idleForSecs) {
        _idleForSecs = idleForSecs;
        emit idleForSecsChanged();
    }
}

void
RuleCondition::setInternetConnectionMode(InternetConnectionMode mode)
{
    if (_internetConnectionMode != mode) {
        _internetConnectionMode = mode;
        emit internetConnectionModeChanged();
    }
}

void
RuleCondition::setChargingState(ChargingState state)
{
    qDebug("RuleCondition::setChargingState(%d) current %d", state, _chargingState);
    if (_chargingState != state) {
        _chargingState = state;
        emit chargingStateChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleCondition &rule)
{
    argument.beginStructure();
    argument << rule.getTimeStart();
    argument << rule.getTimeEnd();
    argument << rule.getDays();
    argument << rule.getLocationCells();
    argument << rule.getLocationCellsTimeout();
    argument << rule.getWlan();
    argument << rule.getWlanTimeout();
    argument << rule.getIdleForSecs();
    argument << rule.nfc();
    argument << int(rule.getInternetConnectionMode());
    argument << int(rule.getChargingState());
    argument << rule.batteryLevel();
    argument << rule.calendar();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleCondition &rule)
{
    QTime     timeStart = rule.getTimeStart();
    QTime     timeEnd = rule.getTimeEnd();
    QList<int> days = rule.getDays().toList();
    QList<int> cells = rule.getLocationCells().toList();
    int     cellsTimeout = rule.getLocationCellsTimeout();
    QList<QString> wlan = rule.getWlan().toList();
    int     wlanTimeout = rule.getWlanTimeout();
    int     idleForSecs = rule.getIdleForSecs();
    RuleConditionNFC nfc;
    int networkMode;
    int chargingState;
    RuleConditionBatteryLevel batteryLevel;
    RuleConditionCalendar calendar;

    argument.beginStructure();
    argument >> timeStart;
    argument >> timeEnd;
    argument >> days;
    argument >> cells;
    argument >> cellsTimeout;
    argument >> wlan;
    argument >> wlanTimeout;
    argument >> idleForSecs;
    argument >> nfc;
    argument >> networkMode;
    argument >> chargingState;
    argument >> batteryLevel;
    argument >> calendar;
    argument.endStructure();

    rule.setTimeStart(timeStart);
    rule.setTimeEnd(timeEnd);
    rule.setDays(QSet<int>::fromList(days));
    rule.setLocationCells(QSet<int>::fromList(cells));
    rule.setLocationCellsTimeout(cellsTimeout);
    rule.setWlan(QSet<QString>::fromList(wlan));
    rule.setWlanTimeout(wlanTimeout);
    rule.setIdleForSecs(idleForSecs);
    rule.setNFC(nfc);
    rule.setInternetConnectionMode((RuleCondition::InternetConnectionMode)networkMode);
    rule.setChargingState((RuleCondition::ChargingState)chargingState);
    rule.setBatteryLevel(batteryLevel);
    rule.setCalendar(calendar);

    return argument;
}
