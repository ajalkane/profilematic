# Add more folders to ship with the application, here
folder_01.source = qml
folder_01.target =
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# IMPROVE: Correct behaviour should be:
#  - UnitTests ran before deploying/running of the executable
#  - If UnitTests fail, no deployment/running of the executable
#  - UnitTest code must not be included in the final executable.
# Uncomment if just unit tests are to be ran:
# CONFIG += qtestlib

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

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += src/main.cpp \
    src/profileclient.cpp \
    src/qmlbackend/qmlbackend.cpp \
    src/qmlbackend/qmldaysmodel.cpp \
    src/model/rule.cpp \
    src/model/rules.cpp \
    src/qmlbackend/qmlrulesmodel.cpp \
    src/qmlbackend/qmlprofilesmodel.cpp \
    src/logic/rulewatch.cpp \
    src/settings.cpp \
    src/logic/ruleactivator.cpp

HEADERS += \
    src/profileclient.h \
    src/qmlbackend/qmlbackend.h \
    src/qmlbackend/qmldaysmodel.h \
    src/model/rule.h \
    src/model/rules.h \
    src/qmlbackend/qmlrulesmodel.h \
    src/qmlbackend/qmlprofilesmodel.h \
    src/logic/rulewatch.h \
    src/settings.h \
    src/logic/ruleactivator.h

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

#OTHER_FILES += \
#    qml/main.qml \
#    qml/ShowRules.qml \
#    profilematic.desktop \
#    profilematic.svg \
#    profilematic.png \
#    qtc_packaging/debian_harmattan/rules \
#    qtc_packaging/debian_harmattan/README \
#    qtc_packaging/debian_harmattan/copyright \
#    qtc_packaging/debian_harmattan/control \
#    qtc_packaging/debian_harmattan/compat \
#    qtc_packaging/debian_harmattan/changelog \
#    qml/UIConstants.js \
#    qml/LabelTitle.qml \
#    qml/LabelStyleSubtitle.qml \
#    qml/SectionHeader.qml \
#    qml/TextFieldWithLabel.qml \
#    qml/LabelStyleTitle.qml \
#    qml/CommonDialog.qml \
#    qml/MySelectionDialog.qml \
#    qml/MyMultiSelectionDialog.qml \
#    qml/MyMultiSelectionDialog.js \
#    qml/dummydata/backendRulesModel.qml \
#    qml/dummydata/backendDaysModel.qml \
#    qml/dummydata/backendProfilesModel.qml \
#    qml/EditRule.qml \
#    qml/ApplicationHeader.qml

#RESOURCES += \
#    res.qrc

qtestlib {
    SOURCES -= src/main.cpp
    SOURCES += tests/main.cpp \
        tests/logic/testrulewatch.cpp
    HEADERS += tests/logic/testrulewatch.h

}

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

# enable booster
#CONFIG += qdeclarative-boostable
#QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
#QMAKE_LFLAGS += -pie -rdynamic

#contains(MEEGO_EDITION,harmattan) {
#    icon.files = profilematic.png
#    icon.path = /usr/share/icons/hicolor/80x80/apps
#    INSTALLS += icon
#}



