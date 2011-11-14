#ifndef PROFILEMATICCLIENT_H
#define PROFILEMATICCLIENT_H

#include <QObject>
#include <QList>

// TODO need common sources or library for this
#include "../../profilematicd/src/model/rule.h"

class ProfileMaticClient : public QObject
{
    Q_OBJECT
public:
    explicit ProfileMaticClient(QObject *parent = 0);

    QList<Rule> getRules() const;
signals:

public slots:

};

#endif // PROFILEMATICCLIENT_H
