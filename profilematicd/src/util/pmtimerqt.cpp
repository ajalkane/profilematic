#include <QDebug>

#include "pmtimerqt.h"
#include "conditionallogging.h"

PmTimer::PmTimer()
{
    IFDEBUG(qDebug() << "PmTimer::constructor");

    connect(&_timer, SIGNAL(timeout()), this, SIGNAL(timeout()));
}
