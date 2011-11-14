#ifndef PROFILEMATICINTERFACE_H
#define PROFILEMATICINTERFACE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>

#include "../model/rule.h"
#include "../settings.h"

class ProfileMaticInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ajalkane.profilematic")
    QList<Rule> *_rules;
public:
    explicit ProfileMaticInterface(QList<Rule> *rules, QObject *parent = 0);
    virtual ~ProfileMaticInterface();

    // Return 0 if success
    int init();
signals:

public slots:
    // Returns id of created rule or empty if error. A new rule is always
    // added as last rule.
    // QString addRule();

    // QStringList getRuleIds();

    QList<Rule> getRules() const;

//    Rule getRule(const QString &rule);

//    // Returns true if success
//    bool deleteRule(const QString &id);

//    // Moves rule to position given by index. The valid
//    // values are in the range 0 - amount of rules.
//    // Returns true if success.
//    int moveRule(const QString &id, int index);

//    // Setters and getters
//    QStringList getRuleIds() const;
//    QString getRuleName(const QString &id) const;
//    bool setRuleName(const QString &id, const QString &name) const;
};

#endif // PROFILEMATICINTERFACE_H
