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
#include "testcalendarentrymatchercondition.h"

TestCalendarEntryMatcherCondition::TestCalendarEntryMatcherCondition()
{
}

void
TestCalendarEntryMatcherCondition::matchSummary() {
    RuleConditionCalendar cond;
    cond.setSummaryMatch("thismatch1 , match2,matc.h3,  ,");

    CalendarEntryMatcherCondition matcher(cond);
    CalendarEntry entry1(QDateTime(), QDateTime(), "thiSmatch1");
    CalendarEntry entry2(QDateTime(), QDateTime(), "tooMATCH2");
    CalendarEntry entry3(QDateTime(), QDateTime(), "MATC.H3too");
    CalendarEntry entry4(QDateTime(), QDateTime(), "nomatch");

    QCOMPARE(matcher.match(entry1), true);
    QCOMPARE(matcher.match(entry2), true);
    QCOMPARE(matcher.match(entry3), true);
    QCOMPARE(matcher.match(entry4), false);
}

void
TestCalendarEntryMatcherCondition::matchSummaryEmpty() {
    RuleConditionCalendar cond;
    cond.setSummaryMatch("");

    CalendarEntryMatcherCondition matcher(cond);
    CalendarEntry entry1(QDateTime(), QDateTime(), "thiSmatch1");
    CalendarEntry entry2(QDateTime(), QDateTime(), "");
    CalendarEntry entry3(QDateTime(), QDateTime(), " ");

    QCOMPARE(matcher.match(entry1), false);
    QCOMPARE(matcher.match(entry2), false);
    QCOMPARE(matcher.match(entry3), false);
}
