/**********************************************************************
 * Copyright 2011 Arto Jalkanen
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
#include <QDateTime>
#include <QSet>

#include "rulewatch.h"

RuleWatch::RuleWatch(const Rules *rules, QObject *parent)
    : QObject(parent), _rules(rules), _targetRuleItem(NULL)
{
    _timer.setSingleShot(true);

    connect(&_timer, SIGNAL(timeout()), this, SLOT(_activateAndReschedule()));
}

void
RuleWatch::refreshWatch() {
    QDateTime now = QDateTime::currentDateTime();
    refreshWatch(now);
}

void
RuleWatch::refreshWatch(const QDateTime &now) {
    qDebug("RuleWatch::refreshWatch(%s)", qPrintable(now.toString()));

    _timer.stop();

    QDateTime nextNearestDateTime;

    qDebug("RuleWatch::refreshWatch size of rules: %d", _rules->size());
    for (Rules::const_iterator i = _rules->constBegin(); i != _rules->constEnd(); ++i) {
        const RuleItem &item = *i;
        if (item.ruleActive) {
            QDateTime nearestFromRule = _nextDateTimeFromRule(now, item);
            if (!nearestFromRule.isNull() && (nextNearestDateTime.isNull() || nearestFromRule < nextNearestDateTime)) {
                qDebug("Setting nearest to %s, was %s",
                       qPrintable(nearestFromRule.toString()),
                       qPrintable(nextNearestDateTime.toString()));
                nextNearestDateTime = nearestFromRule;
                _targetRuleItem = &item;
            }
        }
    }

    if (!nextNearestDateTime.isNull()) {
        quint64 interval = now.msecsTo(nextNearestDateTime);
        qDebug("Now %s", qPrintable(now.toString()));
        qDebug("Scheduling a timer to %s, interval %dms", qPrintable(nextNearestDateTime.toString()), (int)interval);
        _timer.start(interval);
    } else {
        _targetRuleItem = NULL;
        qDebug("No nearest time based rule found");
    }
}

const QTimer *
RuleWatch::timer() const {
    return &_timer;
}

const RuleItem *
RuleWatch::targetRuleItem() const {
    return _targetRuleItem;
}

const Rules *
RuleWatch::rules() const {
    return _rules;
}

QDateTime
RuleWatch::_nextDateTimeFromRule(const QDateTime &from, const RuleItem &rule) const {
    bool isDaysUsable = rule.isDaysRuleUsable();
    bool isTimeStartUsable = rule.isTimeStartRuleUsable();
    if (!isDaysUsable && !isTimeStartUsable) {
        qDebug("QRuleWatch::time(rule %s) Day and time is not usable, returning null date time", qPrintable(rule.rule.getRuleName()));
        return QDateTime();
    }

    if (!isDaysUsable && isTimeStartUsable) {
        qDebug("QRuleWatch::time(rule %s), Day not usable, using timeStart rule %s",
               qPrintable(rule.rule.getRuleName()),
               qPrintable(rule.rule.getTimeStart().toString()));
        QDateTime next = from;
        next.setTime(rule.rule.getTimeStart());
        if (next < from) {
            next = next.addDays(1);
        }
        qDebug("QRuleWatch::time(rule %s), returning %s",
               qPrintable(rule.rule.getRuleName()),
               qPrintable(next.toString()));
        return next;
    }

    // rule.daysActive = true and days not empty always if gets this far
    const QSet<int> &selectedDays = rule.rule.getDays();
    int dayOfWeek = from.date().dayOfWeek();
    // i goes up to 7, so that next week's current day is considered also.
    for (int i = 0; i < 8; ++i) {
        int dayId = (dayOfWeek - 1 + i) % 7;
        bool considerDay = selectedDays.contains(dayId);

        qDebug("QRuleWatch::time(rule %s), considering dayId %d (%d)", qPrintable(rule.rule.getRuleName()), dayId, considerDay);

        if (considerDay) {
            QDateTime next = from;
            next = next.addDays(i);
            if (rule.isTimeStartRuleUsable()) {
                next.setTime(rule.rule.getTimeStart());
            } else {
                next.setTime(QTime(0, 0));
            }
            // Guards against a rule when:
            // - Some days, including current day, are set
            // - No time has not set
            // In other words, a rule that is based on weekdays. In these cases
            // the current day can not be considered as an edge case, but the next day.
            if (next >= from) {
                qDebug("QRuleWatch::time(rule %s), returning %s",
                       qPrintable(rule.rule.getRuleName()),
                       qPrintable(next.toString()));
                return next;
            } else {
                qDebug("QRuleWatch::time(rule %s), continuing to next day because %s < %s",
                       qPrintable(rule.rule.getRuleName()),
                       qPrintable(next.toString()),
                       qPrintable(from.toString()));
            }

        }
    }
    qDebug("QRuleWatch::time(rule %s), returning null QDateTime",
           qPrintable(rule.rule.getRuleName()));
    return QDateTime();
}

void
RuleWatch::_activateAndReschedule() {
    qDebug("RuleWatch::_activateAndReschedule");

    QDateTime now = QDateTime::currentDateTime();

    if (_isTargetRuleActivable()) {
        emit activateRule(*_targetRuleItem);
    }
    refreshWatch(now);
}

bool
RuleWatch::_isTargetRuleActivable() const {
    if (_targetRuleItem && !_targetRuleItem->ruleActive) {
        return false;
    }

    // IMPROVE: as fail safe rule, probably should check that the target
    // rule can be matched for example within a minute
    return true;
}
