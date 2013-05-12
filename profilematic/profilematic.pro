# Add more folders to ship with the application, here
translations.source = i18n
translations.target =

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
    ../profilematicd/src/model/ruleconditionbatterylevel.cpp \
    ../profilematicd/src/model/ruleconditioncalendar.cpp \
    ../profilematicd/src/model/ruleaction.cpp \
    ../profilematicd/src/model/ruleactionapplication.cpp \
    ../profilematicd/src/model/ruleactionalarm.cpp \
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
    src/qmlbackend/qmlruleutil.cpp \
    src/qmlbackend/networkinfo/qmlnetworkinfo.cpp \
    src/qmlbackend/application/qmlapplicationsmodel.cpp \
    src/qmlbackend/application/qmlapplicationsscanner.cpp \
    src/qmlbackend/application/qmlapplicationscannertomodeladapter.cpp \
    src/qmlbackend/qmlsortproxymodel.cpp \
    src/qmlbackend/application/qmlselectedapplicationsmodel.cpp \
    src/qmlbackend/application/qmlapplication.cpp \
    src/qmlbackend/qmlchoisemodel.cpp

HEADERS += \
    src/profileclient.h \
    src/qmlbackend/qmlbackend.h \
    src/qmlbackend/qmldaysmodel.h \
    ../profilematicd/src/model/rule.h \
    ../profilematicd/src/model/ruleaction.h \
    ../profilematicd/src/model/ruleactionapplication.h \
    ../profilematicd/src/model/ruleactionalarm.h \
    ../profilematicd/src/model/ruleconditionnfc.h \
    ../profilematicd/src/model/ruleconditionbatterylevel.h \
    ../profilematicd/src/model/ruleconditioncalendar.h \
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
    src/qmlbackend/qmlruleutil.h \
    src/qmlbackend/networkinfo/qmlnetworkinfo.h \
    src/qmlbackend/application/qmlapplicationsmodel.h \
    src/qmlbackend/application/qmlapplicationsscanner.h \
    src/qmlbackend/application/qmlapplicationscannertomodeladapter.h \
    src/qmlbackend/qmlsortproxymodel.h \
    src/qmlbackend/application/qmlselectedapplicationsmodel.h \
    src/qmlbackend/application/qmlapplication.h \
    src/qmlbackend/qmlchoisemodel.h

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

    # Splash screen
    splash.files = profilematic-splash.png
    splash.path = /opt/profilematic/

    INSTALLS += splash

    qml_item.source = qml/harmattan/qml
    qml_item.target =

} else {
    SOURCES += src/qmlbackend/presence/qmlpresencemodelstub.cpp
    HEADERS += src/qmlbackend/presence/qmlpresencemodelstub.h

    # By default use Harmattan's QML files. For this to work as expected,
    # the theme must be installed. For example see the theme part of:
    # http://meegoharmattandev.blogspot.fi/2011/07/setting-up-qt-sdk-for-meego-harmattan.html
    qml_item.source = qml/harmattan/qml
    qml_item.target =
}

# TRANSLATIONS = i18n/tr_en.ts \
#               i18n/tr_fi.ts

# RESOURCES += resources.qrc

DEPLOYMENTFOLDERS = qml_item translations

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES +=
