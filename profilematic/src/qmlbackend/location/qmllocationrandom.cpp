#include "qmllocationrandom.h"

#include <cstdlib>
#include <time.h>

QmlLocationRandom::QmlLocationRandom(QObject *parent)
    : QmlLocation(parent)
{
    /* initialize random seed: */
    srand(time(NULL));
    _currentCell = rand();

    _timer.setInterval(5 * 1000);
    _timer.setSingleShot(false);
    _timer.start();
    connect(&_timer, SIGNAL(timeout()), this, SLOT(updateCurrentCell()));
}

QmlLocationRandom::~QmlLocationRandom() {}

int
QmlLocationRandom::getCurrentCell() const {
    return _currentCell;
}

void
QmlLocationRandom::updateCurrentCell() {
    _currentCell = rand();
    qDebug("Update current cell called, currentCell now %d", _currentCell);
    emit cellIdChanged(_currentCell);
}
