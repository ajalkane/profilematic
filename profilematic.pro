TEMPLATE = subdirs

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

CONFIG += ordered

SUBDIRS += \
    shared \
    profilematicd profilematic

profilematicd.depends = shared
profilematic.depends = shared

contains(MEEGO_EDITION,harmattan) {
    SUBDIRS += platformutil
}
