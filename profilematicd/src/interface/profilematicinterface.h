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
    void ruleUpdated(const Rule &rule);
    // Rule is always created as the last rule
    void ruleAppended(const Rule &rule);
    void ruleRemoved(const QString &ruleId);
public slots:
    // Returns id of created rule or empty if error. A new rule is always
    // added as last rule.
    // QString addRule();

    // QStringList getRuleIds();

    QList<Rule> getRules() const;

    void updateRule(const Rule &rule);
    // Creates a new rule. Returns the new ruleId. If the given rule
    // already has a ruleId, it is not used.
    QString appendRule(const Rule &rule);
    void removeRule(const QString &ruleId);

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
private:
    int _findRuleIndexById(const Rule::IdType &id) const;
};

#endif // PROFILEMATICINTERFACE_H
