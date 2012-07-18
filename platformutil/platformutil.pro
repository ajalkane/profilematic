TEMPLATE = app
CONFIG += console
CONFIG += qmsystem2
CONFIG += qt-boostable

SOURCES += main.cpp


contains(MEEGO_EDITION,harmattan) {
    target.path = /opt/profilematic/bin
    INSTALLS += target
}
