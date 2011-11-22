#include "profilematicclient.h"

#define PM_PATH "/org/ajalkane/profilematic"
#define PM_SERVICE "org.ajalkane.profilematic"
#define PM_INTERFACE "org.ajalkane.profilematic"

ProfileMaticClient::ProfileMaticClient(QObject *parent) :
    QObject(parent), dbus_iface(NULL)
{
    qDBusRegisterMetaType<Rule>();
    qDBusRegisterMetaType<QList<Rule> >();

    dbus_iface = new QDBusInterface(PM_SERVICE, PM_PATH,
                                    PM_INTERFACE);

    connect(dbus_iface, SIGNAL(ruleUpdated(const Rule &)), this, SIGNAL(ruleUpdated(const Rule &)));
}

ProfileMaticClient::~ProfileMaticClient() {
    delete dbus_iface;
}

QList<Rule>
ProfileMaticClient::getRules() const {
    QDBusReply<QList<Rule> > reply = dbus_iface->call("getRules");
    if (reply.isValid()) {
        QList<Rule> rules = reply.value();
        for (QList<Rule>::const_iterator i = rules.constBegin(); i != rules.constEnd(); ++i) {
            qDebug("ProfileMaticClient: rule id '%s', rule name %s",
                   qPrintable(i->getRuleId()), qPrintable(i->getRuleName()));
        }
        return rules;
    } else {
        QDBusError e = reply.error();
        qDebug("getRules error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }
    return QList<Rule>();
}

void
ProfileMaticClient::updateRule(const Rule &rule) {
    /*QDBusReply<QList<Rule> > reply = */ dbus_iface->call("updateRule", QVariant::fromValue(rule));
    /*if (!reply.isValid()) {
        QDBusError e = reply.error();
        qDebug("updateRule error %s %s %d", qPrintable(e.message()), qPrintable(e.name()), e.type());
    }*/
}
