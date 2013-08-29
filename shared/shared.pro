QT += core dbus
QT -= gui
TARGET = shared
TEMPLATE = lib
CONFIG += staticlib

SOURCES += src/model/rule.cpp \
    src/model/ruleaction.cpp \
    src/model/presencerule.cpp \
    src/model/rulecondition.cpp \
    src/model/ruleconditionnfc.cpp \
    src/model/ruleconditionbatterylevel.cpp \
    src/model/ruleconditioncalendar.cpp \
    src/model/ruleactionapplication.cpp \
    src/model/ruleactionalarm.cpp \
    src/model/ruleconditiontimeinterval.cpp

HEADERS += src/model/rule.h \
    src/model/ruleaction.h \
    src/model/presencerule.h \
    src/model/rulecondition.h \
    src/model/ruleconditionnfc.h \
    src/model/ruleconditionbatterylevel.h \
    src/model/ruleconditioncalendar.h \
    src/model/ruleactionapplication.h \
    src/model/ruleactionalarm.h \
    src/model/ruleconditiontimeinterval.h

!isEmpty(MEEGO_VERSION_MAJOR) {
    message(Building with libaccounts-qt)
    CONFIG += link_pkgconfig
    CONFIG += timed
    PKGCONFIG += accounts-qt
    PKGCONFIG += TelepathyQt4
}
