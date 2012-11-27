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
#include "testcalendarentrymatcherdatetime.h"

TestCalendarEntryMatcherDateTime::TestCalendarEntryMatcherDateTime()
{
}

void
TestCalendarEntryMatcherDateTime::matchDateTime() {

    RuleConditionCalendar cond;
    QDateTime now = QDateTime::fromString("27.09.2011 22:00", "dd.MM.yyyy hh:mm");

    CalendarEntryMatcherDateTime matcher(cond, now);
    CalendarEntry matchStartLimit(now, now.addSecs(60*10), QString());
    CalendarEntry matchEndLimit(now.addSecs(-60*10), now, QString());
    CalendarEntry nomatchStartAfterNow(now.addSecs(1), now.addSecs(2), QString());
    CalendarEntry nomatchEndBeforeNow(now.addSecs(-2), now.addSecs(-1), QString());
    // If start and end are same, end is used as start + 1min, so this should match
    CalendarEntry matchStartAndEndSame(now.addSecs(-1), now.addSecs(-1), QString());

    QCOMPARE(matcher.match(matchStartLimit), true);
    QCOMPARE(matcher.match(matchEndLimit), true);
    QCOMPARE(matcher.match(nomatchStartAfterNow), false);
    QCOMPARE(matcher.match(nomatchEndBeforeNow), false);
    QCOMPARE(matcher.match(matchStartAndEndSame), true);

    // Check nextNearest values
    QCOMPARE(matcher.nextNearestStartOrEnd(now, matchStartLimit), matchStartLimit.end());
    QCOMPARE(matcher.nextNearestStartOrEnd(now, matchEndLimit), QDateTime());
    QCOMPARE(matcher.nextNearestStartOrEnd(now, nomatchStartAfterNow), nomatchStartAfterNow.start());
    QCOMPARE(matcher.nextNearestStartOrEnd(now, nomatchEndBeforeNow), QDateTime());
    QCOMPARE(matcher.nextNearestStartOrEnd(now, matchStartAndEndSame), matchStartAndEndSame.start().addSecs(60));
}
