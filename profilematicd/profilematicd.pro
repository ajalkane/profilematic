#-------------------------------------------------
#
# Project created by QtCreator 2011-11-08T20:36:34
#
#-------------------------------------------------

LIBS += ../shared/libshared.a
INCLUDEPATH += ../shared/src

QT       += core dbus

QT       -= gui

TARGET = profilematicd
CONFIG += console
CONFIG -= app_bundle
CONFIG += qmsystem2
CONFIG += mobility
# For switching Dual/2g/3g
CONFIG += cellular-qt
# For notifications
CONFIG += meegotouch

PKGCONFIG += gq-gconf

MOBILITY += systeminfo
MOBILITY += connectivity

#!isEmpty(MEEGO_VERSION_MAJOR) {
#    MOBILITY += connectivity
#}

# IMPROVE: Correct behaviour should be:
#  - UnitTests ran before deploying/running of the executable
#  - If UnitTests fail, no deployment/running of the executable
#  - UnitTest code must not be included in the final executable.
# Uncomment if just unit tests are to be ran:
# CONFIG += qtestlib

TEMPLATE = app

SOURCES += src/main.cpp \
    src/profileclient.cpp \
    src/configuration.cpp \
    src/interface/profilematicinterface.cpp \
    src/preferences.cpp \
    src/platform/platformutil.cpp \
    src/logic/conditionmanager.cpp \
    src/logic/rulesmanager.cpp \
    src/logic/action.cpp \
    src/logic/conditionmanagertime.cpp \
    src/logic/actionprofile.cpp \
    src/logic/actionflightmode.cpp \
    src/logic/conditionmanagerlocationcell.cpp \
    src/logic/conditionmanagerwlan.cpp \
    src/logic/actionbluetooth.cpp \
    src/logic/actionpowersavingmode.cpp \
    src/logic/actioncellularmode.cpp \
    src/logic/actioncommandline.cpp \
    src/logic/actionstandbyscreenmode.cpp \
    src/logic/actionstatefulbase.cpp \
    src/logic/actionactivateoncebase.cpp \
    src/logic/actionbackgroundconnections.cpp \
    src/logic/conditionmanageridle.cpp \
    src/logic/conditionmanagernfc.cpp \
    src/logic/conditionmanagerinternetconnectionmode.cpp \
    src/logic/conditionmanagercharging.cpp \
    src/logic/conditionmanagerfactory.cpp \
    src/logic/actionfactory.cpp \
    src/logic/conditionmanagerbatterylevel.cpp \
    src/logic/calendar/conditionmanagercalendar.cpp \
    src/platform/calendar/calendarmanager.cpp \
    src/platform/calendar/calendarentry.cpp \
    src/logic/calendar/calendarentrymatchercondition.cpp \
    src/logic/calendar/calendarentrymatcher.cpp \
    src/logic/calendar/calendarentrymatcherdatetime.cpp \
    src/platform/calendar/impl/stub/calendarmanagerstub.cpp \
    src/logic/conditionmanagercacheable.cpp \
    src/logic/conditionmanagercaching.cpp \
    src/util/conditionallogging.cpp \
    src/logic/rulesholder.cpp \
    src/logic/application/actionapplication.cpp \
    src/logic/actiondevicevolume.cpp \
    src/logic/actionalarm.cpp \
    src/logic/actiondevicebrightness.cpp \
    src/logic/conditionmanagertimeinterval.cpp

HEADERS += \
    src/profileclient.h \
    src/configuration.h \
    src/interface/profilematicinterface.h \
    src/preferences.h \
    src/platform/platformutil.h \
    src/logic/conditionmanager.h \
    src/logic/rulesmanager.h \
    src/logic/action.h \
    src/logic/conditionmanagertime.h \
    src/logic/actionprofile.h \
    src/logic/actionflightmode.h \
    src/logic/conditionmanagerlocationcell.h \
    src/logic/conditionmanagerwlan.h \
    src/logic/actionbluetooth.h \
    src/logic/presence/actionpresence.h \
    src/logic/actionpowersavingmode.h \
    src/logic/actioncellularmode.h \
    src/logic/actioncommandline.h \
    src/logic/actionstandbyscreenmode.h \
    src/logic/actionstatefulbase.h \
    src/logic/actionactivateoncebase.h \
    src/logic/actionbackgroundconnections.h \
    src/logic/conditionmanageridle.h \
    src/logic/conditionmanagernfc.h \
    src/logic/conditionmanagerinternetconnectionmode.h \
    src/logic/conditionmanagercharging.h \
    src/logic/conditionmanagerfactory.h \
    src/logic/actionfactory.h \
    src/logic/conditionmanagerbatterylevel.h \
    src/logic/calendar/conditionmanagercalendar.h \
    src/platform/calendar/calendarmanager.h \
    src/platform/calendar/calendarentry.h \
    src/logic/calendar/calendarentrymatchercondition.h \
    src/logic/calendar/calendarentrymatcher.h \
    src/logic/calendar/calendarentrymatcherdatetime.h \
    src/platform/calendar/impl/stub/calendarmanagerstub.cpp \
    src/logic/conditionmanagercacheable.h \
    src/logic/conditionmanagercaching.h \
    src/util/pmtimer.h \
    src/util/conditionallogging.h \
    src/logic/rulesholder.h \
    src/logic/application/actionapplication.h \
    src/logic/actiondevicevolume.h \
    src/logic/actionalarm.h \
    src/logic/actiondevicebrightness.h \
    src/logic/conditionmanagertimeinterval.h

!isEmpty(MEEGO_VERSION_MAJOR) {
    DEFINES += PM_TIMER_USHORT

    SOURCES += src/platform/harmattan/harmattan_platformutil.cpp \
               src/platform/harmattan/volumebarlogic.cpp
    HEADERS += src/platform/harmattan/harmattan_platformutil.h \
               src/platform/harmattan/volumebarlogic.h

}

!isEmpty(MEEGO_VERSION_MAJOR) {
    message(Building with libaccounts-qt)
    CONFIG += link_pkgconfig
    CONFIG += timed
    PKGCONFIG += accounts-qt
    PKGCONFIG += TelepathyQt4

    # Live and learn from OrganizerFeed:
    # Add gq-gconf and libmkcal - we're not using QtMobility's organizer module
    # as it loads the whole calendar into memory
    CONFIG += link_pkgconfig
    PKGCONFIG += libmkcal
    # Device volume
    PKGCONFIG += dbus-1 \
                dbus-glib-1

    SOURCES += src/logic/presence/actionpresenceimpl.cpp \
               src/platform/calendar/impl/mkcal/calendarmanagermkcal.cpp
    HEADERS += src/logic/presence/actionpresenceimpl.h \
               src/platform/calendar/impl/mkcal/calendarmanagermkcal.h

    # Notification icon
    data.files = data/profilematic40.png
    data.path = /opt/profilematic/data

    INSTALLS += data

} else {
    SOURCES += src/logic/presence/actionpresencestub.cpp
    HEADERS += src/logic/presence/actionpresencestub.h
}

contains(DEFINES, PM_TIMER_USHORT) {
    SOURCES += src/util/pmtimerushort.cpp
    HEADERS += src/util/pmtimerushort.h
} else {
    SOURCES += src/util/pmtimerqt.cpp
    HEADERS += src/util/pmtimerqt.h
}

qtestlib {
    SOURCES -= src/main.cpp
    SOURCES += tests/main.cpp \
        tests/logic/testconditionmanagertime.cpp \
        tests/logic/signalcounter.cpp \
        tests/logic/testcalendarentrymatchercondition.cpp \
        tests/logic/testcalendarentrymatcherdatetime.cpp \
        tests/logic/testconditionmanagercalendar.cpp \
        tests/logic/testconditionmanagercaching.cpp \
        tests/util/testpmtimer.cpp


    HEADERS += tests/logic/testconditionmanagertime.h \
        tests/logic/signalcounter.h \
        tests/logic/testcalendarentrymatchercondition.h \
        tests/logic/testcalendarentrymatcherdatetime.h \
        tests/logic/testconditionmanagercalendar.h \
        tests/logic/testconditionmanagercaching.h \
        tests/util/testpmtimer.h
}

#!isEmpty(MEEGO_VERSION_MAJOR) {
    target.path = /opt/profilematic/bin
    INSTALLS += target
#}

daemonconf.path = /etc/init/apps
daemonconf.files = profilematicd.conf

INSTALLS += daemonconf
