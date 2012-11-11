#-------------------------------------------------
#
# Project created by QtCreator 2011-11-08T20:36:34
#
#-------------------------------------------------

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
# CONFIG += meegotouch

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
    src/model/rule.cpp \
    src/model/ruleaction.cpp \
    src/model/presencerule.cpp \
    src/interface/profilematicinterface.cpp \
    src/preferences.cpp \
    src/platform/platformutil.cpp \
    src/logic/conditionmanager.cpp \
    src/logic/rulesmanager.cpp \
    src/logic/action.cpp \
    src/logic/conditionmanagertime.cpp \
    src/logic/actionprofile.cpp \
    src/logic/actionflightmode.cpp \
    src/logic/actionchain.cpp \
    src/logic/conditionmanagerchain.cpp \
    src/logic/conditionmanagerlocationcell.cpp \
    src/logic/conditionmanagerwlan.cpp \
    src/logic/actionbluetooth.cpp \
    src/logic/actionpowersavingmode.cpp \
    src/logic/actioncellularmode.cpp \
    src/logic/actioncommandline.cpp \
    src/logic/actionstandbyscreenmode.cpp \
    src/model/rulecondition.cpp \
    src/logic/actionstatefulbase.cpp \
    src/logic/actionactivateoncebase.cpp \
    src/logic/actionbackgroundconnections.cpp \
    src/logic/conditionmanageridle.cpp \
    src/model/ruleconditionnfc.cpp \
    src/logic/conditionmanagernfc.cpp \
    src/logic/conditionmanagerinternetconnectionmode.cpp \
    src/logic/conditionmanagercharging.cpp \
    src/logic/conditionmanagerfactory.cpp \
    src/logic/actionfactory.cpp \
    src/model/ruleconditionbatterylevel.cpp

HEADERS += \
    src/profileclient.h \
    src/configuration.h \
    src/model/rule.h \
    src/model/ruleaction.h \
    src/model/presencerule.h \
    src/interface/profilematicinterface.h \
    src/preferences.h \
    src/platform/platformutil.h \
    src/logic/conditionmanager.h \
    src/logic/rulesmanager.h \
    src/logic/action.h \
    src/logic/conditionmanagertime.h \
    src/logic/actionprofile.h \
    src/logic/actionflightmode.h \
    src/logic/actionchain.h \
    src/logic/conditionmanagerchain.h \
    src/logic/conditionmanagerlocationcell.h \
    src/logic/conditionmanagerwlan.h \
    src/logic/actionbluetooth.h \
    src/logic/presence/actionpresence.h \
    src/logic/actionpowersavingmode.h \
    src/logic/actioncellularmode.h \
    src/logic/actioncommandline.h \
    src/logic/actionstandbyscreenmode.h \
    src/model/rulecondition.h \
    src/logic/actionstatefulbase.h \
    src/logic/actionactivateoncebase.h \
    src/logic/actionbackgroundconnections.h \
    src/logic/conditionmanageridle.h \
    src/model/ruleconditionnfc.h \
    src/logic/conditionmanagernfc.h \
    src/logic/conditionmanagerinternetconnectionmode.h \
    src/logic/conditionmanagercharging.h \
    src/logic/conditionmanagerfactory.h \
    src/logic/actionfactory.h \
    src/model/ruleconditionbatterylevel.h

!isEmpty(MEEGO_VERSION_MAJOR) {
    SOURCES += src/platform/harmattan/harmattan_platformutil.cpp
    HEADERS += src/platform/harmattan/harmattan_platformutil.h
}

!isEmpty(MEEGO_VERSION_MAJOR) {
    message(Building with libaccounts-qt)
    CONFIG += link_pkgconfig
    PKGCONFIG += accounts-qt
    PKGCONFIG += TelepathyQt4

    SOURCES += src/logic/presence/actionpresenceimpl.cpp
    HEADERS += src/logic/presence/actionpresenceimpl.h
} else {
    SOURCES += src/logic/presence/actionpresencestub.cpp
    HEADERS += src/logic/presence/actionpresencestub.h
}

qtestlib {
    SOURCES -= src/main.cpp
    SOURCES += tests/main.cpp \
        tests/logic/testconditionmanagerchain.cpp \
        tests/logic/testconditionmanagertime.cpp \
        tests/logic/signalcounter.cpp

    HEADERS += tests/logic/testconditionmanagerchain.h \
        tests/logic/testconditionmanagertime.h \
        tests/logic/signalcounter.h
}

#!isEmpty(MEEGO_VERSION_MAJOR) {
    target.path = /opt/profilematic/bin
    INSTALLS += target
#}

daemonconf.path = /etc/init/apps
daemonconf.files = profilematicd.conf

INSTALLS += daemonconf
