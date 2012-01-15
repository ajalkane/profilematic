#ifndef CONDITIONMANAGERTIME_H
#define CONDITIONMANAGERTIME_H

#include <QDateTime>
#include <QTime>
#include <QTimer>
#include <QPair>

#include "conditionmanager.h"

class ConditionManagerTime : public ConditionManager
{
    typedef ConditionManager super;
    QTimer _timer;
    QDateTime _nextNearestDateTime;
    QDateTime _refreshTime;

    QDateTime _calculateNextEnd(const QDateTime &dateTimeStart, const QTime &timeStart, const QTime &timeEnd) const;
    QPair<QDateTime, bool> _nextDateTimeFromRule(const QDateTime &from, const Rule &rule) const;
    bool _refresh(const Rule &rule, const QDateTime &now);

public:
    ConditionManagerTime(QObject *parent = 0);

    virtual void startRefresh();
    virtual bool refresh(const Rule &rule);
    virtual void endRefresh();

    // These functions only needed for unit tests
    const QTimer *timer() const {
        return &_timer;
    }
    void startRefresh(const QDateTime &now);
};

#endif // CONDITIONMANAGERTIME_H
