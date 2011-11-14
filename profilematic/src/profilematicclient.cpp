#include <QtDBus/QtDBus>

#include "profilematicclient.h"

#define PM_PATH "/org/ajalkane/profilematic"
#define PM_SERVICE "org.ajalkane.profilematic"
#define PM_INTERFACE "org.ajalkane.profilematic"

ProfileMaticClient::ProfileMaticClient(QObject *parent) :
    QObject(parent)
{
}

QList<Rule>
ProfileMaticClient::getRules() const {
    QDBusInterface dbus_iface(PM_SERVICE, PM_PATH,
                              PM_INTERFACE);

    QDBusReply<QList<Rule> > reply = dbus_iface.call("getRules");
    if (reply.isValid()) {
        QList<Rule> rules = reply.value();
        for (QList<Rule>::const_iterator i = rules.constBegin(); i != rules.constEnd(); ++i) {
            qDebug("ProfileMaticClient: rule name %s", qPrintable(i->getRuleName()));
        }
        return rules;
    } else {
        QDBusError e = reply.error();
        qDebug("getRules error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }
    return QList<Rule>();
}
