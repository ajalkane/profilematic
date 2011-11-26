#ifndef PROFILEMATICINTERFACE_H
#define PROFILEMATICINTERFACE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>

#include "../model/rule.h"
#include "../configuration.h"
#include "../preferences.h"
#include "../logic/rulewatch.h"

class ProfileMaticInterface : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.ajalkane.profilematic")
    RuleWatch   *_ruleWatch;
    QList<Rule> *_rules;
    Preferences *_preferences;

public:
    explicit ProfileMaticInterface(RuleWatch *ruleWatch, QList<Rule> *rules, Preferences *preferences, QObject *parent = 0);
    virtual ~ProfileMaticInterface();

    // Return 0 if success
    int init();
signals:
    void ruleUpdated(const Rule &rule);
    // Rule is always created as the last rule
    void ruleAppended(const Rule &rule);
    void ruleRemoved(const QString &ruleId);
    void ruleMoved(const QString &ruleId, int toIndex);
    void activeChanged(bool isActive);
public slots:
    QList<Rule> getRules() const;

    void updateRule(const Rule &rule);
    // Creates a new rule. Returns the new ruleId. If the given rule
    // already has a ruleId, it is not used.
    QString appendRule(const Rule &rule);
    void removeRule(const QString &ruleId);
    void moveRule(const QString &ruleId, int index);

    // Returns true if rules are watched
    bool isActive() const;
    void setActive(bool active);

private:
    int _findRuleIndexById(const Rule::IdType &id) const;
    void _rulesChanged();
    void _preferencesChanged();
};

#endif // PROFILEMATICINTERFACE_H
