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
/**
 * NOTE: the functionality of this file has been mostly adjusted from
 * Philip Lorenz's OrganizerFeed code. It has been most helpful especially
 * in the corner cases where API doesn't work as expected.
 */
#include <QDebug>

#include "calendarmanagermkcal.h"

#include <extendedcalendar.h>
#include <extendedstorage.h>

#define SECONDS_PER_DAY (24 * 60 * 60)

CalendarManagerMkCal::CalendarManagerMkCal(QObject *parent)
    : CalendarManager(parent), _calendarBackendPtr(0), _storagePtr(0)
{
    _inactivityCalendarCloseTimer.setSingleShot(true);

    connect(&_inactivityCalendarCloseTimer, SIGNAL(timeout()), this, SLOT(_reopenCalendar()));
}

void
CalendarManagerMkCal::_ensureCalendarInitialized() {
    if (_calendarBackendPtr.isNull()) {
        _calendarBackendPtr =
                mKCal::ExtendedCalendar::Ptr(new mKCal::ExtendedCalendar(KDateTime::Spec::LocalZone()));
        _storagePtr =
                mKCal::ExtendedStorage::Ptr(mKCal::ExtendedCalendar::defaultStorage(_calendarBackendPtr));
        _storagePtr->registerObserver(this);
        _storagePtr->open();
    }
}

namespace {
    void logExpandedIncidence(const mKCal::ExtendedCalendar::ExpandedIncidence &expandedIncident) {
        const mKCal::ExtendedCalendar::ExpandedIncidenceValidity
                &incidenceValidity = expandedIncident.first;
        KCalCore::Incidence::Ptr incidence = expandedIncident.second;

        qDebug() << "Incidence start " << incidenceValidity.dtStart;
        qDebug() << "Incidence end " << incidenceValidity.dtEnd;
        qDebug() << "Incidence summary " << incidence->summary();
        qDebug() << "Incidence location " << incidence->location();
        qDebug() << "Incidence categories " << incidence->categoriesStr();
    }

    void logNotebooks(mKCal::ExtendedStorage::Ptr storagePtr) {
        mKCal::Notebook::List allNotebooks(storagePtr->notebooks());
        foreach(const mKCal::Notebook::Ptr& currNotebook, allNotebooks) {
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
    }

} // namespace

void
CalendarManagerMkCal::_fixIncidenceDates(KCalCore::Incidence::Ptr incidence, QDateTime &dtStart, QDateTime &dtEnd) {
    if (incidence->type() == KCalCore::IncidenceBase::TypeEvent) {
        KDateTime dateStart = KDateTime(dtStart).toLocalZone();
        KDateTime dateEnd = KDateTime(dtEnd).toLocalZone();
        // If an event ends on the next date the incidenceValidity.dtEnd value is wrong
        // (always set to 23:59:59 of the next day). Fix this by manually retrieving
        // the end time.
        if (!incidence->recurs()) {
            dateEnd = incidence.staticCast<KCalCore::Event>()->dtEnd().toLocalZone();
        } else {
            dateEnd = incidence->endDateForStart(dateStart);
        }

        dtEnd = dateEnd.dateTime();
    }

    // Handle malformed entries whose start date is newer than their end date
    if (dtEnd < dtStart) {
        dtEnd = dtStart;
    }
}

QList<CalendarEntry>
CalendarManagerMkCal::loadCalendarEntries(const QDate &startDate, const QDate &endDate) {
    _ensureCalendarInitialized();

    // logNotebooks(_storagePtr);

    // Ignore load failures as load() simply checks if at least one new entry
    // has been loaded (which must not neccesarily be the case).
    _storagePtr->load(startDate, endDate);
    _storagePtr->loadRecurringIncidences();

    KCalCore::Incidence::List incidences =
            _calendarBackendPtr->incidences(startDate, endDate);

    // For some reason if startDate should be included in expansions we need
    // to start expanding one day beforehands.
    mKCal::ExtendedCalendar::ExpandedIncidenceList
            incidenceList = _calendarBackendPtr->expandRecurrences(&incidences,
                                                                   KDateTime(startDate.addDays(-1)),
                                                                   KDateTime(endDate));
    qDebug() << "Expanded incidences size" << incidences.size();
    QList<CalendarEntry> calendarEntries;
    foreach(const mKCal::ExtendedCalendar::ExpandedIncidence &expandedIncident,
            incidenceList) {
        const mKCal::ExtendedCalendar::ExpandedIncidenceValidity
                &incidenceValidity = expandedIncident.first;
        KCalCore::Incidence::Ptr incidence = expandedIncident.second;

        QDateTime dtStart = incidenceValidity.dtStart;
        QDateTime dtEnd   = incidenceValidity.dtEnd;
        qDebug() << "Loaded incidence from calendar:"
                 << dtStart.toString()
                 << dtEnd.toString()
                 << incidence->uid()
                 << incidence->type();

        if (incidence->type() != KCalCore::IncidenceBase::TypeEvent)
            continue;

        QString calendarUid = _calendarBackendPtr->notebook(incidence);
        mKCal::Notebook::Ptr notebook = _storagePtr->notebook(calendarUid);

        _fixIncidenceDates(incidence, dtStart, dtEnd);
        if (!notebook)
            continue;
        if (!notebook->isVisible() || !notebook->eventsAllowed())
            continue;

        calendarEntries << CalendarEntry(incidenceValidity.dtStart, incidenceValidity.dtEnd, incidence->summary(), incidence->location());
    }

    // Start inactivity timer to clear the loaded calendar to save memory
    if (!_inactivityCalendarCloseTimer.isActive()) {
        _inactivityCalendarCloseTimer.start(SECONDS_PER_DAY * 1000);
        qDebug() << QDateTime::currentDateTime().toString() << "CalendarManagerMkCal::starting invactivity close timer";
    }

    return calendarEntries;
}

void
CalendarManagerMkCal::_reopenCalendar() {
    qDebug() << QDateTime::currentDateTime().toString() << "CalendarManagerMkCal::_reopenCalendar";
    // Only reopen if calendar was open
    if (_calendarBackendPtr.isNull()) {
        qDebug() << "CalendarManagerMkCal::_reopenCalendar calendar is not open, doing nothing";
        return;
    }

    // Obtain a reference to the old entries - they will be closed after the
    // new instance has been loaded so we don't miss any updates which
    // happen during reload.
    mKCal::ExtendedCalendar::Ptr oldCalendarBackendPtr = _calendarBackendPtr;
    mKCal::ExtendedStorage::Ptr oldStoragePtr = _storagePtr;

    _calendarBackendPtr.clear();
    _storagePtr.clear();

    _ensureCalendarInitialized();

    if (oldCalendarBackendPtr)
        oldCalendarBackendPtr->close();
    if (oldStoragePtr)
        oldStoragePtr->close();

    qDebug() << "CalendarManagerMkCal::_reopenCalendar done";
}

void
CalendarManagerMkCal::closeCalendar() {
    qDebug() << "CalendarManagerMkCal::closeCalendar";
    if (!_calendarBackendPtr.isNull()) {
        qDebug() << "CalendarManagerMkCal::closeCalendar calendar was open, closing";
        _calendarBackendPtr->close();
        _calendarBackendPtr.clear();
        _storagePtr->close();
        _storagePtr.clear();
    }
}

/**
  * Something inside the storage has changed.
  */
void
CalendarManagerMkCal::storageModified(mKCal::ExtendedStorage *storage,
                                   const QString &info)
{
    Q_UNUSED(info)
    Q_UNUSED(storage)

    qDebug() << QDateTime::currentDateTime().toString() << "CalendarManagerMkCal::storageModified: info" << info;

    emit onCalendarChanged();
}

void
CalendarManagerMkCal::storageProgress(mKCal::ExtendedStorage *storage, const QString &info)
{
    Q_UNUSED(storage)
    Q_UNUSED(info)
}

void
CalendarManagerMkCal::storageFinished(mKCal::ExtendedStorage *storage, bool error, const QString &info)
{
    Q_UNUSED(storage)
    Q_UNUSED(error)
    Q_UNUSED(info)
}
