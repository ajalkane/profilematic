#include "conditionmanagertime.h"

ConditionManagerTime::ConditionManagerTime()
{
    _timer.setSingleShot(true);

    connect(&_timer, SIGNAL(timeout()), this, SIGNAL(refreshNeeded()));
}

void
ConditionManagerTime::startRefresh() {
    super::startRefresh();
    _timer.stop();
}

void
ConditionManagerTime::refresh(const QList<Rule> &rules) {
    QDateTime now = QDateTime::currentDateTime();
    refresh(rules, now);
}

void
ConditionManagerTime::refresh(const QList<Rule> &rules, const QDateTime &now) {
    QDateTime nextNearestDateTime;

    qDebug("ConditionManagerTime::refresh size of rules: %d", rules.size());
    for (QList<Rule>::const_iterator i = rules.constBegin(); i != rules.constEnd(); ++i) {
        const Rule &item = *i;
        // if (item.ruleActive) {
            QPair<QDateTime, bool> p = _nextDateTimeFromRule(now, item);
            QDateTime nearestFromRule = p.first;
            bool matching = p.second;
            if (matching) {
                _matchingRules.append(item);
                qDebug("ConditionManagerTime::refresh adding to matching, size now %d", _matchingRules.size());
            }
            if (!nearestFromRule.isNull() && (nextNearestDateTime.isNull() || nearestFromRule < nextNearestDateTime)) {
                qDebug("Setting nearest to %s, was %s",
                       qPrintable(nearestFromRule.toString()),
                       qPrintable(nextNearestDateTime.toString()));
                nextNearestDateTime = nearestFromRule;
            }
        // }
    }

    if (!nextNearestDateTime.isNull()) {
        quint64 interval = now.msecsTo(nextNearestDateTime);
        qDebug("Now %s", qPrintable(now.toString()));
        qDebug("Scheduling a timer to %s, interval %dms", qPrintable(nextNearestDateTime.toString()), (int)interval);
        _timer.start(interval);
    } else {
        qDebug("No nearest time based rule found");
    }

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
