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
MOBILITY += systeminfo

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
    src/logic/rulewatch.cpp \
    src/logic/ruleactivator.cpp \
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
    src/logic/conditionmanagerwlan.cpp

HEADERS += \
    src/profileclient.h \
    src/configuration.h \
    src/model/rule.h \
    src/logic/rulewatch.h \
    src/logic/ruleactivator.h \
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
    src/logic/conditionmanagerwlan.h

!isEmpty(MEEGO_VERSION_MAJOR) {
    SOURCES += src/platform/harmattan/harmattan_platformutil.cpp
    HEADERS += src/platform/harmattan/harmattan_platformutil.h
}

qtestlib {
    SOURCES -= src/main.cpp
    SOURCES += tests/main.cpp \
        tests/logic/testrulewatch.cpp \
        tests/logic/testconditionmanagerchain.cpp \
        tests/logic/testconditionmanagertime.cpp \
        tests/logic/signalcounter.cpp

    HEADERS += tests/logic/testrulewatch.h \
        tests/logic/testconditionmanagerchain.h \
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



















