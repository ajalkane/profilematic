#include "ruleactionapplication.h"

RuleActionApplication::RuleActionApplication(QObject *parent)
    : QObject(parent)
{
    _init();
}

RuleActionApplication::RuleActionApplication(const RuleActionApplication &o, QObject *parent)
    : QObject(parent)
{
    _launchers = o._launchers;

    _init();
}

RuleActionApplication &
RuleActionApplication::operator=(const RuleActionApplication &o)
{
    _launchers = o._launchers;

    return *this;
}

void
RuleActionApplication::_init() {
    connect(this, SIGNAL(launchersChanged()),       this, SIGNAL(changed()));
}

void
RuleActionApplication::setLaunchers(const QSet<QString> &launchers) {
    _launchers = launchers;
    emit launchersChanged();
}

// For QML
QVariantList
RuleActionApplication::getLaunchersQml() const {
    QVariantList launchers;
    for (QSet<QString>::const_iterator i = _launchers.constBegin(); i != _launchers.constEnd(); ++i) {
        launchers << *i;
    }
    return launchers;

}

void
RuleActionApplication::setLaunchersQml(const QVariantList &launchers) {
    QSet<QString> launchersSet;
    for (QVariantList::const_iterator i = launchers.constBegin(); i != launchers.constEnd(); ++i) {
        QString launcher = i->toString();
        // Weed out empty launchers if somehow they get through
        if (!launcher.trimmed().isEmpty()) {
            launchersSet << i->toString();
        }
    }
    setLaunchers(launchersSet);
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleActionApplication &rule)
{
    argument.beginStructure();
    argument << rule.getLaunchers();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleActionApplication &rule)
{
    QList<QString> launchers;

    argument.beginStructure();
    argument >> launchers;
    argument.endStructure();

    rule.setLaunchers(QSet<QString>::fromList(launchers));

    return argument;
}
