#ifndef PROFILEMATICCLIENT_H
#define PROFILEMATICCLIENT_H

#include <QObject>
#include <QList>
#include <QtDBus/QtDBus>

// TODO need common sources or library for this
#include "../../profilematicd/src/model/rule.h"

class ProfileMaticClient : public QObject
{
    Q_OBJECT

    QDBusInterface *dbus_iface;
public:
    explicit ProfileMaticClient(QObject *parent = 0);
    ~ProfileMaticClient();

    QList<Rule> getRules() const;
    void updateRule(const Rule &rule);
signals:
    void ruleUpdated(const Rule &rule);

public slots:
};

#endif // PROFILEMATICCLIENT_H
