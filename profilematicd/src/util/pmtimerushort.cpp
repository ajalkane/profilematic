#include <climits>

#include "pmtimerushort.h"

PmTimer::PmTimer()
    : _minimumInterval(-1), _maximumInterval(-1)
{
    connect(&_timer, SIGNAL(error(QSystemAlignedTimer::AlignedTimerError)), this, SIGNAL(error(int)));
    connect(&_timer, SIGNAL(timeout()), this, SLOT(_timeout()));
}

void
PmTimer::_start(int minimumInterval, int maximumInterval) {
    if (maximumInterval > USHRT_MAX) {
        _overflowedMinimumTarget = QDateTime::currentDateTime().addSecs(minimumInterval);
        int intermediaryMaximumInterval = USHRT_MAX;
        int intermediaryMinimumInterval = intermediaryMaximumInterval- (maximumInterval - minimumInterval);
        // Tries to keep the difference between minimum and maximum the same to better use
        // the aligned timer, but if the difference is too big to handle, just use the same value for both
        if (intermediaryMinimumInterval < 0) {
            intermediaryMinimumInterval = intermediaryMaximumInterval;
        }

        qDebug("PmTimerUShort::start(%d, %d): unsigned short overflow, setting intermediary interval %d - %d",
               minimumInterval, maximumInterval,
               intermediaryMinimumInterval, intermediaryMaximumInterval);
        _timer.start(intermediaryMinimumInterval,
                     intermediaryMaximumInterval);
    } else {
        _overflowedMinimumTarget = QDateTime();
        _timer.start(minimumInterval, maximumInterval);
    }
}

void
PmTimer::start(int minimumInterval, int maximumInterval) {
    _minimumInterval = minimumInterval;
    _maximumInterval = maximumInterval;
    _start(minimumInterval, maximumInterval);
}

void
PmTimer::_timeout() {
    if (_overflowedMinimumTarget.isNull()) {
        emit timeout();
    } else {
        QDateTime now = QDateTime::currentDateTime();
        int minimumInterval = now.secsTo(_overflowedMinimumTarget);
        qDebug("%s PmTimerUShort::_timeout() delaying timeout, minimumInterval now %d", qPrintable(now.toString()), minimumInterval);
        if (minimumInterval <= 0) {
            emit timeout();
        } else {
            _start(minimumInterval, _maximumInterval - (_minimumInterval - minimumInterval));
        }
    }
}

