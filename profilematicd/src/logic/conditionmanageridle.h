#ifndef CONDITIONMANAGERIDLE_H
#define CONDITIONMANAGERIDLE_H

#include <QSystemAlignedTimer>

QTM_USE_NAMESPACE

#include "conditionmanager.h"
#include "../platform/platformutil.h"

class ConditionManagerIdle : public ConditionManager
{
    Q_OBJECT

public:
    ConditionManagerIdle(QObject *parent = 0);
    virtual ~ConditionManagerIdle();

    virtual void startRefresh();
    virtual bool refresh(const RuleCondition &rule);
    virtual void matchedRule(const RuleCondition &rule);
    virtual void endRefresh();

public slots:
    void userActivityIdleChanged(bool isIdle);
};

#endif // CONDITIONMANAGERIDLE_H
