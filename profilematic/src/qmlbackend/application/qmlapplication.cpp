#include "qmlapplication.h"

QmlApplication::QmlApplication() {
}

QmlApplication::QmlApplication(const QmlApplication &o) {
    (*this) = o;
}

QmlApplication &
QmlApplication::operator=(const QmlApplication &o) {
    this->setApplicationLauncher(o.applicationLauncher());
    this->setApplicationName(o.applicationName());
    this->setIconUri(o.iconUri());
    return *this;
}
