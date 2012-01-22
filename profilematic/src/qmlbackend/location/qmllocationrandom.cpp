/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
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
#include "qmllocationrandom.h"

#include <cstdlib>
#include <time.h>

QmlLocationRandom::QmlLocationRandom(QObject *parent)
    : QmlLocation(parent)
{
    /* initialize random seed: */
    srand(time(NULL));
    _currentCell = rand();
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

void
QmlLocationRandom::monitorCellIdChange(bool monitor) {
    if (monitor) {
        _timer.setInterval(5 * 1000);
        _timer.setSingleShot(false);
        _timer.start();
        connect(&_timer, SIGNAL(timeout()), this, SLOT(updateCurrentCell()), Qt::UniqueConnection);
    } else {
        _timer.stop();
        disconnect(&_timer, SIGNAL(timeout()), this, SLOT(updateCurrentCell()));
    }
}
