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
#ifndef TESTCONDITIONMANAGERCALENDAR_H
#define TESTCONDITIONMANAGERCALENDAR_H

#include <QObject>
#include <QtTest/QtTest>

#include "../../src/logic/calendar/conditionmanagercalendar.h"
#include "../../src/model/rule.h"

class MockCalendarManager;
class MockPlatformUtil;

class TestConditionManagerCalendar : public QObject
{
    Q_OBJECT

    MockCalendarManager *_cm;
    MockPlatformUtil *_pu;
public:
    TestConditionManagerCalendar();

private slots:
    void init();

    void conditionSetForMatching();
    void match_testCalendarLoading();
    void match_nextNearestDateTime_emptyCalendar_nextSetToNextDay();
    void match_nextNearestDateTime_afterNoMoreConditionsMatch_nextSetToNextDay();
    void match_nextNearestDateTime_oneConditionBefore();
    void match_nextNearestDateTime_oneConditionBeforeAndThenAtEnd();
    void match_nextNearestDateTime_oneConditionBeforeRoundToSecond();
    void match_nextNearestDateTime_oneConditionAtStart();
    void match_nextNearestDateTime_oneConditionAtEnd();
    void match_nextNearestDateTime_oneConditionInMiddle();
    void match_nextNearestDateTime_oneConditionBothMatchLaterEndTimeCloser();
    void match_nextNearestDateTime_oneConditionBothMatchEarlierEndTimeCloser();

    void match_nextNearestDateTime_multiConditionFirstMatchSecondNearest();
    void match_nextNearestDateTime_multiConditionSecondMatchFirstNearest();

    void matchInvalidated_signalTest();

};

#endif // TESTCONDITIONMANAGERCALENDAR_H
