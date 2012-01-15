#ifndef CONDITIONMANAGERLOCATIONCELL_H
#define CONDITIONMANAGERLOCATIONCELL_H

#include <QObject>
#include <QSet>

#include "conditionmanager.h"
#include "../platform/platformutil.h"

class ConditionManagerLocationCell : public ConditionManager {
    Q_OBJECT

    PlatformUtil *_platformUtil;
    QSet<int> _watchedCellIds;
    int _currentCellId;
public:
    ConditionManagerLocationCell(PlatformUtil *platformUtil, QObject *parent = 0);
    virtual ~ConditionManagerLocationCell();

    virtual void startRefresh();
    virtual bool refresh(const Rule &rule);
    virtual void endRefresh();

public slots:
    void cellIdChanged(int cellId);
};

#endif // CONDITIONMANAGERLOCATIONCELL_H
