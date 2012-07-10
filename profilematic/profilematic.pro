# Add more folders to ship with the application, here
qml_harmattan.source = qml
qml_harmattan.target =
DEPLOYMENTFOLDERS = qml_harmattan

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT+= declarative dbus
symbian:TARGET.UID3 = 0xEEDCE602

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable
CONFIG += mobility
MOBILITY += systeminfo
MOBILITY += connectivity

# Uncomment to simulate changing cell id in UI. Is not used if MEEGO_MAJOR_VERSION is
# set for build to avoid accidental usage on real device.
CONFIG += profilematic_simulated_cellid

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += src/main.cpp \
    src/profileclient.cpp \
    src/qmlbackend/qmlbackend.cpp \
    src/qmlbackend/qmldaysmodel.cpp \
    ../profilematicd/src/model/rule.cpp \
    ../profilematicd/src/model/rulecondition.cpp \
    ../profilematicd/src/model/ruleconditionnfc.cpp \
    ../profilematicd/src/model/ruleaction.cpp \
    ../profilematicd/src/model/presencerule.cpp \
    src/qmlbackend/qmlrulesmodel.cpp \
    src/qmlbackend/qmlprofilesmodel.cpp \
    src/profilematicclient.cpp \
    src/qmlbackend/qmllocation.cpp \
    src/qmlbackend/presence/qmlpresencemodel.cpp \
    src/qmlbackend/nfc/qmlnfcmobility.cpp \
    src/qmlbackend/qmlconditioneditmodel.cpp \
    src/qmlbackend/qmlboolfiltermodel.cpp \
    src/qmlbackend/qmlbaseruleeditmodel.cpp \
    src/qmlbackend/qmlactioneditmodel.cpp \
    src/qmlbackend/qmlruleutil.cpp

HEADERS += \
    src/profileclient.h \
    src/qmlbackend/qmlbackend.h \
    src/qmlbackend/qmldaysmodel.h \
    ../profilematicd/src/model/rule.h \
    ../profilematicd/src/model/ruleaction.h \
    ../profilematicd/src/model/ruleconditionnfc.h \
    ../profilematicd/src/model/rulecondition.h \
    ../profilematicd/src/model/presencerule.h \
    src/qmlbackend/qmlrulesmodel.h \
    src/qmlbackend/qmlprofilesmodel.h \
    src/profilematicclient.h \
    src/qmlbackend/qmllocation.h \
    src/qmlbackend/presence/qmlpresencemodel.h \
    src/qmlbackend/nfc/qmlnfcmobility.h \
    src/qmlbackend/qmlconditioneditmodel.h \
    src/qmlbackend/qmlboolfiltermodel.h \
    src/qmlbackend/qmlbaseruleeditmodel.h \
    src/qmlbackend/qmlactioneditmodel.h \
    src/qmlbackend/qmlruleutil.h

# To avoid mistakes, do not use the simulated cellId on a device build.
!profilematic_simulated_cellid|!isEmpty(MEEGO_VERSION_MAJOR) {
    message(Building with mobility cellId)
    SOURCES += src/qmlbackend/location/qmllocationmobility.cpp
    HEADERS += src/qmlbackend/location/qmllocationmobility.h
} else {
    message(Building with random cellId)
    DEFINES += PROFILEMATIC_SIMULATED_CELLID
    SOURCES += src/qmlbackend/location/qmllocationrandom.cpp
    HEADERS += src/qmlbackend/location/qmllocationrandom.h
}

!isEmpty(MEEGO_VERSION_MAJOR) {
    message(Building with libaccounts-qt)
    # CONFIG += link_pkgconfig is already provided by qdeclarative-boostable.prf
    # and setting it again leads to build failures.
    PKGCONFIG += accounts-qt

    SOURCES += src/qmlbackend/presence/qmlpresencemodelimpl.cpp
    HEADERS += src/qmlbackend/presence/qmlpresencemodelimpl.h
} else {
    SOURCES += src/qmlbackend/presence/qmlpresencemodelstub.cpp
    HEADERS += src/qmlbackend/presence/qmlpresencemodelstub.h
}

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES +=















