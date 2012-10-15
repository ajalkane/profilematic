#include <climits>

#include "systemalignedtimer.h"

SystemAlignedTimer::SystemAlignedTimer()
    : _minimumInterval(-1), _maximumInterval(-1)
{
    connect(&_timer, SIGNAL(error(QSystemAlignedTimer::AlignedTimerError)), this, SIGNAL(error(QSystemAlignedTimer::AlignedTimerError)));
    connect(&_timer, SIGNAL(timeout()), this, SLOT(_timeout()));
}

void
SystemAlignedTimer::_start(int minimumInterval, int maximumInterval) {
    if (maximumInterval > USHRT_MAX) {
        _overflowedMinimumInterval = QDateTime::currentDateTime();
        _overflowedMinimumInterval.addSecs(minimumInterval);
        int intermediaryMinimumInterval = qMin(minimumInterval, USHRT_MAX);
        int intermediaryMaximumInterval = qMin(maximumInterval, USHRT_MAX);
        qDebug("SystemAlignedTimer::start(%d, %d): unsigned short overflow, setting intermediary interval %d - %d",
               minimumInterval, maximumInterval,
               intermediaryMinimumInterval, intermediaryMaximumInterval);
        _timer.start(intermediaryMinimumInterval,
                     intermediaryMaximumInterval);
    } else {
        _overflowedMinimumInterval = QDateTime();
        _timer.start(minimumInterval, maximumInterval);
    }
}

void
SystemAlignedTimer::start(int minimumInterval, int maximumInterval) {
    _minimumInterval = minimumInterval;
    _maximumInterval = maximumInterval;
    _start(minimumInterval, maximumInterval);
}

void
SystemAlignedTimer::_timeout() {
    if (_overflowedMinimumInterval.isNull()) {
        emit timeout();
    } else {
        QDateTime now = QDateTime::currentDateTime();
        int minimumInterval = now.secsTo(_overflowedMinimumInterval);
        qDebug("SystemAlignedTimer::_timeout() delaying timeout, minimumInterval now %d", minimumInterval);
        if (minimumInterval <= 0) {
            emit timeout();
        } else {
            _start(minimumInterval, _maximumInterval - (_minimumInterval - minimumInterval));
        }
    }
}

