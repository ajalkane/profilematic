#include "pmtimerqt.h"

PmTimer::PmTimer()
{
    connect(&_timer, SIGNAL(timeout()), this, SIGNAL(timeout()));
}
