#-------------------------------------------------
#
# Project created by QtCreator 2011-11-08T20:36:34
#
#-------------------------------------------------

QT       += core dbus

QT       -= gui

TARGET = profilematicd
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += qmsystem2

TEMPLATE = app


SOURCES += src/main.cpp \
    src/profileclient.cpp \
    src/configuration.cpp \
    src/model/rule.cpp \
    src/logic/rulewatch.cpp \
    src/logic/ruleactivator.cpp \
    src/interface/profilematicinterface.cpp \
    src/preferences.cpp \
    src/platform/platformutil.cpp

HEADERS += \
    src/profileclient.h \
    src/configuration.h \
    src/model/rule.h \
    src/logic/rulewatch.h \
    src/logic/ruleactivator.h \
    src/interface/profilematicinterface.h \
    src/preferences.h \
    src/platform/platformutil.h

!isEmpty(MEEGO_VERSION_MAJOR) {
    SOURCES += src/platform/harmattan/harmattan_platformutil.cpp
    HEADERS += src/platform/harmattan/harmattan_platformutil.h
}

#!isEmpty(MEEGO_VERSION_MAJOR) {
    target.path = /opt/profilematic/bin
    INSTALLS += target
#}

daemonconf.path = /etc/init/apps
daemonconf.files = profilematicd.conf

INSTALLS += daemonconf


























