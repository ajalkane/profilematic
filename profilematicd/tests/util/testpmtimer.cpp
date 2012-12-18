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
#include <QDebug>
#include <QDateTime>

#include "testpmtimer.h"
#include "../logic/signalcounter.h"

TestPmTimer::TestPmTimer()
{
}

void
TestPmTimer::signalTest() {
    SignalCounter signalTarget;
    PmTimer timer;

    timer.setSingleShot(true);

    QCOMPARE(signalTarget.numSignal, 0);
    QCOMPARE(timer.isActive(), false);

    connect(&timer, SIGNAL(timeout()), &signalTarget, SLOT(onSignal()));

    QDateTime now = QDateTime::currentDateTime();
    QDateTime expectTimeAtleast = now.addSecs(1);
    expectTimeAtleast = QDateTime(expectTimeAtleast.date(), QTime(expectTimeAtleast.time().hour(), expectTimeAtleast.time().minute(), expectTimeAtleast.time().second()));

    timer.start(1, 1);

    QCOMPARE(timer.isActive(), true);

    qDebug() << "Now" << now.toString("ddd MMM d yyyy hh:mm:ss.zzz");
    qDebug() << "ExpectTimeAtLeast" << expectTimeAtleast.toString("ddd MMM d yyyy hh:mm:ss.zzz");
    QTest::qWait(5 * 1000);

    QCOMPARE(signalTarget.numSignal, 1);
    qDebug() << signalTarget.signalDateTime.toString("ddd MMM d yyyy hh:mm:ss.zzz") << ">=" << expectTimeAtleast.toString("ddd MMM d yyyy hh:mm:ss.zzz");
    QVERIFY(signalTarget.signalDateTime >= expectTimeAtleast);
}

// IMPROVE: exceptional cases, no days, no startTime, no endTime
