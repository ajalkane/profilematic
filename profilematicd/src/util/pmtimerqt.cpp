#include <QDebug>

#include "pmtimerqt.h"

PmTimer::PmTimer()
{
    qDebug() << "PmTimer::constructor";

    connect(&_timer, SIGNAL(timeout()), this, SIGNAL(timeout()));
}
