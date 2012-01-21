#ifndef QMLLOCATIONRANDOM_H
#define QMLLOCATIONRANDOM_H

#include <QTimer>
#include "../qmllocation.h"

class QmlLocationRandom : public QmlLocation
{
    Q_OBJECT

    QTimer _timer;
    int _currentCell;

public:
    QmlLocationRandom(QObject *parent = 0);
    virtual ~QmlLocationRandom();

    virtual int getCurrentCell() const;
public slots:
    void updateCurrentCell();
    void monitorCellIdChange(bool monitor);
};

#endif // QMLLOCATIONRANDOM_H
