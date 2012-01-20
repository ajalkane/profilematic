#ifndef CONDITIONMANAGERTIME_H
#define CONDITIONMANAGERTIME_H

#include <QDateTime>
#include <QTime>
//#include <QTimer>
#include <QPair>
#include <QSystemAlignedTimer>

QTM_USE_NAMESPACE

#include "conditionmanager.h"

class ConditionManagerTime : public ConditionManager
{
    typedef ConditionManager super;
    // QTimer _timer;
    QSystemAlignedTimer _timer;
    int _timerIntervalMaxAddition;
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
    const QSystemAlignedTimer *timer() const {
        return &_timer;
    }
    // Minimum interval in msecs
    inline int minimumIntervalMsec() const {
        return _timer.minimumInterval() * 1000;
    }
    // Maximum interval in msecs
    inline int maximumIntervalMsec() const {
        return _timer.maximumInterval() * 1000;
    }
    inline void setTimerMaxIntervalAddition(int add) {
        _timerIntervalMaxAddition = add;
    }

    void startRefresh(const QDateTime &now);
};

#endif // CONDITIONMANAGERTIME_H
