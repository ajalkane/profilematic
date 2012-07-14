TEMPLATE = app
CONFIG += console
CONFIG -= qt
CONFIG += qmsystem2

SOURCES += main.cpp


contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/profilematic/bin
    INSTALLS += target
}
