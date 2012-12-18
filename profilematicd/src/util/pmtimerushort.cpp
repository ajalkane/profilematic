#include <climits>

#include <QDebug>

#include "pmtimerushort.h"

PmTimer::PmTimer()
    : _minimumInterval(-1), _maximumInterval(-1)
{
    qDebug() << "PmTimerUShort::constructor";
    connect(&_timer, SIGNAL(error(QSystemAlignedTimer::AlignedTimerError)), this, SLOT(_error(QSystemAlignedTimer::AlignedTimerError)));
    connect(&_timer, SIGNAL(timeout()), this, SLOT(_timeout()));
}

PmTimer::~PmTimer() {}

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
        qDebug("%s PmTimerUShort::_start() starting normal timeout with %d/%d", qPrintable(QDateTime::currentDateTime().toString()), minimumInterval, maximumInterval);
        _overflowedMinimumTarget = QDateTime();
        _timer.start(minimumInterval, maximumInterval);
    }
}

void
PmTimer::start(int minimumInterval, int maximumInterval) {
    qDebug("%s PmTimerUShort::start() called with %d/%d", qPrintable(QDateTime::currentDateTime().toString()), minimumInterval, maximumInterval);
    if (_timer.isActive()) {
        qDebug("PmTimerUShort::start() was active, stopping");
        _timer.stop();
    }
    _minimumInterval = minimumInterval;
    _maximumInterval = maximumInterval;
    _start(minimumInterval, maximumInterval);
}

void
PmTimer::_timeout() {
    if (_overflowedMinimumTarget.isNull()) {
        qDebug("%s PmTimerUShort::_timeout() emitting timeout", qPrintable(QDateTime::currentDateTime().toString()));
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

void
PmTimer::_error(QSystemAlignedTimer::AlignedTimerError err) {
    qDebug() << QDateTime::currentDateTime().toString() << "PmTimerUShort:_error" << err;
    emit error((int)err);
}
