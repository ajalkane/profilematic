#ifndef CONDITIONMANAGERIDLE_H
#define CONDITIONMANAGERIDLE_H

#include <QDateTime>
#include <QTime>
#include <QSystemAlignedTimer>

QTM_USE_NAMESPACE

#include "conditionmanager.h"
#include "../platform/platformutil.h"

class ConditionManagerIdle : public ConditionManager
{
    Q_OBJECT

    QSystemAlignedTimer _timer;
    int _currentMinIdleSecs;
    int _nextMinWakeupSecs;
    int _currentIdleMode;
    bool _hasActive;
    QDateTime _idleStartTime;
    QDateTime _refreshTime;

    void _scheduleWakeup(int secsToWakeUp);

public:
    ConditionManagerIdle(QObject *parent = 0);
    virtual ~ConditionManagerIdle();

    virtual void startRefresh();
    virtual bool refresh(const Rule::IdType &, const RuleCondition &rule);
    virtual void endRefresh();

private slots:
    void userActivityIdleChanged(bool isIdle);
};

#endif // CONDITIONMANAGERIDLE_H
