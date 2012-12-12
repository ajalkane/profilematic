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

#ifndef PMTIMERQT_H
#define PMTIMERQT_H

#include <QObject>
#include <QTimer>

class PmTimer : public QObject
{
    Q_OBJECT

    QTimer _timer;
public:
    PmTimer();

    inline void setSingleShot(bool single) {
        _timer.setSingleShot(single);
    }

    inline void start(int minimumInterval, int maximumInterval) {
        Q_UNUSED(maximumInterval)

        _timer.start(minimumInterval);
    }

    inline void stop() {
        _timer.stop();
    }

    inline bool isActive() const {
        return _timer.isActive();
    }

    // 0 for no error, non-zero for error
    inline int lastError() const {
        return 0;
    }

    inline int minimumInterval() const {
        return _timer.interval();
    }
    inline int maximumInterval() const {
        return _timer.interval();
    }

signals:
    void timeout();
    // never emitted.from this object
    void error(int error);
};

#endif // PMTIMERQT_H
