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

#include "calendarentrymatchercondition.h"

CalendarEntryMatcherCondition::CalendarEntryMatcherCondition(const RuleConditionCalendar &condition)
{
    _createRegExpFromKeywords(_regExpSummary, condition.getSummaryMatch());
}

void
CalendarEntryMatcherCondition::_createRegExpFromKeywords(QRegExp &regExp, const QString &keywordsStr) {
    QStringList keywords = keywordsStr.split(QRegExp("\\s*,\\s*"), QString::SkipEmptyParts);
    QString regExpStr;
    foreach (const QString &keyword, keywords) {
        if (!regExpStr.isEmpty()) {
            regExpStr += "|";
        }
        regExpStr += QString("%1").arg(QRegExp::escape(keyword));
    }

    qDebug() << "CalendarEntryMatcherSummary::constructor created regexp" << regExpStr;
    if (!regExpStr.isEmpty()) {
        regExp = QRegExp(regExpStr, Qt::CaseInsensitive);
    } else {
        // Create regExp that won't match.
        regExp = QRegExp(" ^");
    }
}

bool
CalendarEntryMatcherCondition::match(const CalendarEntry &entry) {
    return _regExpSummary.indexIn(entry.summary()) != -1;
}
