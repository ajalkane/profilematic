#include "conditionmanagertime.h"

ConditionManagerTime::ConditionManagerTime()
{
    _timer.setSingleShot(true);

    connect(&_timer, SIGNAL(timeout()), this, SIGNAL(refreshNeeded()));
}

void
ConditionManagerTime::startRefresh(const QDateTime &now) {
    _timer.stop();
    _nextNearestDateTime = QDateTime();
    _refreshTime = now;
}

void
ConditionManagerTime::startRefresh() {
    startRefresh(QDateTime::currentDateTime());
}

void
ConditionManagerTime::endRefresh() {
    if (!_nextNearestDateTime.isNull()) {
        quint64 interval = _refreshTime.msecsTo(_nextNearestDateTime);
        qDebug("Now %s", qPrintable(_refreshTime.toString()));
        qDebug("Scheduling a timer to %s, interval %dms", qPrintable(_nextNearestDateTime.toString()), (int)interval);
        _timer.start(interval);
    } else {
        qDebug("No nearest time based rule found");
    }
}

bool
ConditionManagerTime::refresh(const Rule &rule) {
    return _refresh(rule, _refreshTime);
}

bool
ConditionManagerTime::_refresh(const Rule &rule, const QDateTime &now) {
    QPair<QDateTime, bool> p = _nextDateTimeFromRule(now, rule);
    QDateTime nearestFromRule = p.first;
    bool matching = p.second;

    qDebug("ConditionManagerTime::refresh rule %s/%s match %d",
           qPrintable(rule.getRuleId()),
           qPrintable(rule.getRuleName()),
           matching);

    if (!nearestFromRule.isNull() && (_nextNearestDateTime.isNull() || nearestFromRule < _nextNearestDateTime)) {
        qDebug("Setting nearest to %s, was %s",
               qPrintable(nearestFromRule.toString()),
               qPrintable(_nextNearestDateTime.toString()));
        _nextNearestDateTime = nearestFromRule;
    }
    return matching;
}

QPair<QDateTime, bool>
ConditionManagerTime::_nextDateTimeFromRule(const QDateTime &from, const Rule &rule) const {
    bool isDaysUsable = rule.isDaysRuleUsable();
    bool isTimeStartUsable = rule.isTimeStartRuleUsable();
    bool isTimeEndUsable = rule.isTimeEndRuleUsable();

    if (!isDaysUsable || !isTimeStartUsable || !isTimeEndUsable) {
        // Return as time matching if time start and time end not set
        bool matching = (!isTimeStartUsable && !isTimeEndUsable);
        qDebug("ConditionManagerTime::time(rule %s) Day or time is not usable, returning null date time (matching %d)",
               qPrintable(rule.getRuleName()), matching);
        return qMakePair(QDateTime(), matching);
    }

    // rule.daysActive = true and days not empty always if gets this far
    const QSet<int> &selectedDays = rule.getDays();
    int dayOfWeek = from.date().dayOfWeek();
    // i goes up to 7, so that next week's current day is considered also.
    for (int i = 0; i < 8; ++i) {
        int dayId = (dayOfWeek - 1 + i) % 7;
        bool considerDay = selectedDays.contains(dayId);

        qDebug("ConditionManagerTime::time(rule %s), considering dayId %d (%d)", qPrintable(rule.getRuleName()), dayId, considerDay);

        if (considerDay) {
            QDateTime nextStart = from;
            nextStart = nextStart.addDays(i);
            nextStart.setTime(rule.getTimeStart());
            QDateTime nextEnd = _calculateNextEnd(nextStart, rule.getTimeStart(), rule.getTimeEnd());
            qDebug("ConditionManagerTime::from(%s), nextStart(%s), nextEnd(%s)",
                   qPrintable(from.toString()),
                   qPrintable(nextStart.toString()),
                   qPrintable(nextEnd.toString()));

            // Guards against a rule when:
            // - Some days, including current day, are set
            // - No time has been set
            // In other words, a rule that is based on weekdays. In these cases
            // the current day can not be considered as an edge case, but the next day.
            if (nextStart > from) {
                qDebug("ConditionManagerTime::time(rule %s), matching next timeStart returning %s",
                       qPrintable(rule.getRuleName()),
                       qPrintable(nextStart.toString()));
                return qMakePair(nextStart, false);
            } else if (nextEnd > from) {
                qDebug("ConditionManagerTime::time(rule %s), matching next timeEnd returning %s",
                       qPrintable(rule.getRuleName()),
                       qPrintable(nextEnd.toString()));
                return qMakePair(nextEnd, true);
            }

        }
    }
    qDebug("ConditionManagerTime::time(rule %s), returning null QDateTime",
           qPrintable(rule.getRuleName()));
    // Means no time based rules
    return qMakePair(QDateTime(), true);
}

QDateTime
ConditionManagerTime::_calculateNextEnd(const QDateTime &dateTimeStart, const QTime &timeStart, const QTime &timeEnd) const {
    QDateTime nextEnd(dateTimeStart);
    nextEnd.setTime(timeEnd);
    if (timeEnd < timeStart) {
        nextEnd = nextEnd.addDays(1);
    }
    return nextEnd;
}
