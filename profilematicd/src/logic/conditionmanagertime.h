#ifndef CONDITIONMANAGERTIME_H
#define CONDITIONMANAGERTIME_H

#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <QPair>

#include "conditionmanager.h"

class ConditionManagerTime : public ConditionManager
{
    QTimer _timer;

    QDateTime _calculateNextEnd(const QDateTime &dateTimeStart, const QTime &timeStart, const QTime &timeEnd) const;
    QPair<QDateTime, bool> _nextDateTimeFromRule(const QDateTime &from, const Rule &rule) const;

public:
    ConditionManagerTime();

    virtual void refresh(const QList<Rule> &rules);

    // These functions only needed for unit tests
    const QTimer *timer() const {
        return &_timer;
    }
    void refresh(const QList<Rule> &rules, const QDateTime &dateTime);
};

#endif // CONDITIONMANAGERTIME_H
