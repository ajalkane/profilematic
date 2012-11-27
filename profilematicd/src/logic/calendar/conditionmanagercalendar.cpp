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

#include "../../platform/platformutil.h"

#include "conditionmanagercalendar.h"
#include "calendarentrymatchercondition.h"

ConditionManagerCalendar::ConditionManagerCalendar(QObject *parent)
    : ConditionManager(parent), _entriesLoaded(false)
{    
    qDebug("ConditionManagerCalendar::constructor");

    _timer.setSingleShot(true);

    connect(&_timer, SIGNAL(timeout()), this, SIGNAL(askRefresh()));

//    QDate startDate = QDate(2012, 11, 20);
//    QDate endDate = QDate(2012, 11, 23);

//    CalendarManager *cm = PlatformUtil::instance()->createCalendarManager();
//    QList<CalendarEntry> entries = cm->loadCalendarEntries(startDate, endDate);

//    foreach (const CalendarEntry &entry, entries) {
//        qDebug() << "CalendarEntry start" << entry.start();
//        qDebug() << "CalendarEntry end" << entry.end();
//        qDebug() << "CalendarEntry summary " << entry.summary();
//    }

/*
    foreach (QString manager, QOrganizerManager::availableManagers()) {
             qDebug("ConditionManagerCalendar::availableManager %s", qPrintable(manager));
    }
    QOrganizerManager defaultManager;
    qDebug("ConditionManagerCalendar::defaultManager %s", qPrintable(defaultManager.managerName()));

    mKCal::ExtendedCalendar::Ptr m_calendarBackendPtr;
    mKCal::ExtendedStorage::Ptr m_storagePtr;

    m_calendarBackendPtr =
            mKCal::ExtendedCalendar::Ptr(new mKCal::ExtendedCalendar(KDateTime::Spec::LocalZone()));
    m_storagePtr =
            mKCal::ExtendedStorage::Ptr(mKCal::ExtendedCalendar::defaultStorage(m_calendarBackendPtr));
    // m_storagePtr->registerObserver(this);
    m_storagePtr->open();

    // loadEventsFromBackend
    quint32 m_maxLookAhead = 24 * 3600;
    mKCal::Notebook::List allNotebooks(m_storagePtr->notebooks());
     foreach(const mKCal::Notebook::Ptr& currNotebook, allNotebooks) {
         // Update look-ahead
         // m_maxLookAhead = qMax(m_maxLookAhead, calendarSettings->lookAhead());
         // TODO print something about the notebooks to see what works
         qDebug() << "Notebook uid " << currNotebook->uid();
         qDebug() << "Notebook name " << currNotebook->name();
         qDebug() << "Notebook description " << currNotebook->description();
         qDebug() << "Notebook color " << currNotebook->color();
         qDebug() << "Notebook isShared " << currNotebook->isShared();
         qDebug() << "Notebook isMaster " << currNotebook->isMaster();
         qDebug() << "Notebook isSynchronized " << currNotebook->isSynchronized();
         qDebug() << "Notebook isVisible " << currNotebook->isVisible();
         qDebug() << "Notebook account " << currNotebook->account();
         qDebug() << "Notebook isDefault " << currNotebook->isDefault();
         qDebug() << "Notebook syncProfile " << currNotebook->syncProfile();
     }

     QDate startDate = QDate(2012, 11, 20);
     QDate endDate = QDate(2012, 11, 23);

     // Ignore load failures as load() simply checks if at least one new entry
      // has been loaded (which must not neccesarily be the case).
      m_storagePtr->load(startDate, endDate);
      m_storagePtr->loadRecurringIncidences();

      KCalCore::Incidence::List incidences =
              m_calendarBackendPtr->incidences(startDate, endDate);

      qDebug() << "Incidences size" << incidences.size();
      foreach(const KCalCore::Incidence::Ptr& incidence, incidences) {
      }


      // For some reason if startDate should be included in expansions we need
      // to start expanding one day beforehands.
      mKCal::ExtendedCalendar::ExpandedIncidenceList
              incidenceList = m_calendarBackendPtr->expandRecurrences(&incidences,
                                                                      KDateTime(startDate.addDays(-1)),
                                                                      KDateTime(endDate));
      qDebug() << "Expanded incidences size" << incidences.size();
      foreach(const mKCal::ExtendedCalendar::ExpandedIncidence &expandedIncident,
              incidenceList) {
          const mKCal::ExtendedCalendar::ExpandedIncidenceValidity
                          &incidenceValidity = expandedIncident.first;
          KCalCore::Incidence::Ptr incidence = expandedIncident.second;

          qDebug() << "Incidence start " << incidenceValidity.dtStart;
          qDebug() << "Incidence end " << incidenceValidity.dtEnd;
          qDebug() << "Incidence summary " << incidence->summary();
          qDebug() << "Incidence location " << incidence->location();
          qDebug() << "Incidence categories " << incidence->categoriesStr();

          // qDebug() << "Expanded Incidence description " << expandedIncident.description();
    } */

}

void
ConditionManagerCalendar::startRefresh() {
    _timer.stop();
    _nextNearestDateTime = QDateTime();
    _refreshTime = QDateTime(QDate(2012, 11, 21), QTime(8,45)); // QDateTime::currentDateTime();
}

void
ConditionManagerCalendar::_updateNextNearestDateTime(const CalendarEntry &entry, const CalendarEntryMatcherDateTime &matcher) {
    QDateTime fromEntry = matcher.nextNearestStartOrEnd(_refreshTime, entry);
    if (fromEntry.isValid() && fromEntry < _nextNearestDateTime) {
        _nextNearestDateTime = fromEntry;
    }
}

bool
ConditionManagerCalendar::refresh(const Rule::IdType &, const RuleCondition &condition) {
    const RuleConditionCalendar &condCal = condition.calendar();

    if (!condCal.isValid()) {
        qDebug("ConditionManagerCalendar::refresh calendar options not set or invalid, matches");
        return true;
    }

    _ensureCalendarEntriesLoaded();

    bool matches = false;
    CalendarEntryMatcherCondition matcherSummary(condCal);
    CalendarEntryMatcherDateTime matcherDateTime(condCal, _refreshTime);

    foreach (CalendarEntry entry, _entries) {
        if (matcherSummary.match(entry)) {
            _updateNextNearestDateTime(entry, matcherDateTime);
            if (matcherDateTime.match(entry)) {
                matches = true;
                break;
            }
        }
    }

    return matches;
}

void
ConditionManagerCalendar::endRefresh() {
    if (!_nextNearestDateTime.isNull()) {
        int interval = _refreshTime.secsTo(_nextNearestDateTime);
        qDebug("ConditionManagerCalendar: Now %s", qPrintable(_refreshTime.toString()));
        qDebug("ConditionManagerCalendar: Scheduling a timer to %s, interval %ds", qPrintable(_nextNearestDateTime.toString()), (int)interval);
        _timer.start(interval); // , interval + _timerIntervalMaxAddition);
    } else {
        qDebug("ConditionManagerCalendar: No nearest calendar based rule found");
    }

}

void
ConditionManagerCalendar::_ensureCalendarEntriesLoaded() {
    if (!_entriesLoaded) {
        QDate startDate = QDate(2012, 11, 21);
        QDate endDate = QDate(2012, 11, 21);

        CalendarManager *cm = PlatformUtil::instance()->createCalendarManager();
        _entries = cm->loadCalendarEntries(startDate, endDate);

        _entriesLoaded = true;
    }
}

void
ConditionManagerCalendar::askRefresh() {
    qDebug("%s ConditionManagerCalendar::askRefresh", qPrintable(_refreshTime.toString()));
    emit refreshNeeded();
}
