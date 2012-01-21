#ifndef QMLLOCATIONMOBILITY_H
#define QMLLOCATIONMOBILITY_H

#include <QSystemNetworkInfo>

QTM_USE_NAMESPACE

#include "../qmllocation.h"

// Location services for platforms that support QtMobility
class QmlLocationMobility : public QmlLocation
{
    Q_OBJECT

    QSystemNetworkInfo *_source;
public:
    QmlLocationMobility(QObject *parent = 0);
    ~QmlLocationMobility();

    virtual int getCurrentCell() const;

public slots:
    void monitorCellIdChange(bool monitor);
};

#endif // QMLLOCATIONMOBILITY_H
