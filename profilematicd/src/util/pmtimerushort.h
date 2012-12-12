/**********************************************************************
 * Copyright 2012 Arto Jalkanen
 *
 * This file is part of ProfileMatic.
 *
 * ProfileMatic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProfileMatic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProfileMatic.  If not, see <http://www.gnu.org/licenses/>
**/

#ifndef PMTIMERUSHORT_H
#define PMTIMERUSHORT_H

#include <QObject>
#include <QDateTime>

#include <QSystemAlignedTimer>

QTM_USE_NAMESPACE

/**
 * This class is intended to fix the behaviour of QSystemAlignedTimer on MeeGo/Maemo
 * platforms. When the waiting time exceeds unsigned short in seconds, QSystemAlignedTimer
 * does not work as intended. This is due to the underlying implementation relying on
 * heartbeat service which handles only unsigned short waiting intervals.
 *
 * Only the part of the API that's needed by ProfileMatic is implemented.
 */
class PmTimer : public QObject
{
    Q_OBJECT

    QSystemAlignedTimer _timer;
    // Contains the real minimum target time if overflowed
    QDateTime _overflowedMinimumTarget;
    // The real minimum and maximum interval until timeout
    int _minimumInterval;
    int _maximumInterval;

    void _start(int minimumInterval, int maximumInterval);
public:
    PmTimer();

    inline void setSingleShot(bool single) {
        _timer.setSingleShot(single);
    }

    void start(int minimumInterval, int maximumInterval);

    inline void stop() {
        _timer.stop();
    }

    inline bool isActive() const {
        return _timer.isActive();
    }

    // 0 for no error, non-zero for error
    inline int lastError() const {
        return _timer.lastError();
    }

    inline int minimumInterval() const {
        return _minimumInterval;
    }
    inline int maximumInterval() const {
        return _maximumInterval;
    }

private slots:
    void _timeout();

signals:
    void timeout();
    void error(int error);
};

#endif // PMTIMERUSHORT_H
