/**********************************************************************
 * Copyright 2011 Arto Jalkanen
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
#include <QtCore/QCoreApplication>

#include "logic/testconditionmanagertime.h"
#include "logic/testconditionmanagercalendar.h"
#include "logic/testconditionmanagercaching.h"
#include "logic/testcalendarentrymatchercondition.h"
#include "logic/testcalendarentrymatcherdatetime.h"
#include "util/testpmtimer.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Q_UNUSED(app)

    int retval(0);

    QList<QObject *> tests;
    tests << new TestCalendarEntryMatcherCondition();
    tests << new TestCalendarEntryMatcherDateTime();
    tests << new TestConditionManagerCalendar();
    tests << new TestConditionManagerCaching();
    tests << new TestConditionManagerTime();
    tests << new TestPmTimer();

    foreach (QObject *test, tests) {
        retval += QTest::qExec(test, argc, argv);
        delete test;
    }
    qDebug("Tests exiting with return value %d", retval);
    return (retval ? 1 : 0);
}
