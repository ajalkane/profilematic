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

#include "../../util/conditionallogging.h"
#include "calendarentrymatchercondition.h"

CalendarEntryMatcherCondition::CalendarEntryMatcherCondition(const RuleConditionCalendar &condition)
{
    bool someMatch = false;
    someMatch |= _createRegExpFromKeywords(_regExpSummary, condition.getSummaryMatch());
    someMatch |= _createRegExpFromKeywords(_regExpLocation, condition.getLocationMatch());
    if (!someMatch) {
        // Create regExp that won't match.
        IFDEBUG(qDebug() << "CalendarEntryMatcherCondition::CalendarEntryMatcherCondition creating non matching regexp");
        _regExpSummary = QRegExp(" ^");
    }
}

bool
CalendarEntryMatcherCondition::_createRegExpFromKeywords(QRegExp &regExp, const QString &keywordsStr) {
    QStringList keywords = keywordsStr.split(QRegExp("\\s*,\\s*"), QString::SkipEmptyParts);
    QString regExpStr;
    foreach (const QString &keyword, keywords) {
        if (!regExpStr.isEmpty()) {
            regExpStr += "|";
        }
        regExpStr += QString("%1").arg(QRegExp::escape(keyword));
    }

    IFDEBUG(qDebug() << "CalendarEntryMatcherSummary::constructor created regexp" << regExpStr);
    if (!regExpStr.isEmpty()) {
        regExp = QRegExp(regExpStr, Qt::CaseInsensitive);
        return true;
    } else {
        // Create a regexp that always matches
        regExp = QRegExp(".*");
    }
    return false;
}

bool
CalendarEntryMatcherCondition::match(const CalendarEntry &entry) {
    //    qDebug() << "regExpSummary" << _regExpSummary;
    //    qDebug() << "regExpLocation" << _regExpLocation;
    //    qDebug() << "summary" << _regExpSummary.indexIn(entry.summary());
    //    qDebug() << "location" << _regExpLocation.indexIn(entry.location());

    return _regExpSummary.indexIn(entry.summary()) != -1
        && _regExpLocation.indexIn(entry.location()) != -1;
}
