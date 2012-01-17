#ifndef CONDITIONMANAGERLOCATIONCELL_H
#define CONDITIONMANAGERLOCATIONCELL_H

#include <QObject>
#include <QSet>
#include <QSystemNetworkInfo>

QTM_USE_NAMESPACE

#include "conditionmanager.h"
#include "../platform/platformutil.h"

class ConditionManagerLocationCell : public ConditionManager {
    Q_OBJECT

    QSystemNetworkInfo _networkInfo;

    QSet<int> _watchedCellIds;
    int _currentCellId;
    bool _ruleMayMatch;

public:
    ConditionManagerLocationCell(QObject *parent = 0);
    virtual ~ConditionManagerLocationCell();

    virtual void startRefresh();
    virtual bool refresh(const Rule &rule);
    virtual void endRefresh();

    void monitorCellId(bool monitor);

public slots:
    void cellIdChanged(int cellId);
};

#endif // CONDITIONMANAGERLOCATIONCELL_H
