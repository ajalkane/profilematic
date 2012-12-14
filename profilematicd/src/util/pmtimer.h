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
#ifndef PMTIMER_H
#define PMTIMER_H

/**
 * This header is included to use Qt timers on second boundaries
 * on different platforms. QSystemAlignedTimer is the preferred
 * one, but since it does not work on all platforms, or works
 * incorrectly, separate instances can be provided to different
 * platforms.
 *
 * All the implementations must adhere to the interface provided
 * by QSystemAlignedTimer, with the subset needed by ProfileMatic.
 * By default, an implementation based on QTimer is provided.
 *
 * All implementations must provide class named PmTimer.
 *
 * Note: QSystemAlignedTimer and QTimer behave differently when start()
 * is called if the timer was already active (isActive()). QSystemAlignedTimer
 * would disregard the new start (at least in the case of singleShot(true) which
 * is used by Profilematic). QTimer would cancel the previous timer and start
 * the new. Implementations of PmTimer must provide similar functionality as
 * QTimer regards to this.
 */

#ifdef PM_TIMER_USHORT
    #include "pmtimerushort.h"
#else
    #include "pmtimerqt.h"
#endif // PM_TIMER

#endif // PMTIMER_H
