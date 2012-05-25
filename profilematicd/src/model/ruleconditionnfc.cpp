#include "ruleconditionnfc.h"

RuleConditionNFC::RuleConditionNFC(QObject *parent)
    : QObject(parent), _toggleCondition(false)
{
    _init();
}

RuleConditionNFC::RuleConditionNFC(const RuleConditionNFC &o, QObject *parent)
    : QObject(parent), _toggleCondition(false)
{
    _uids = o._uids;
    _toggleCondition = o._toggleCondition;

    _init();
}

RuleConditionNFC &
RuleConditionNFC::operator=(const RuleConditionNFC &o)
{
    _uids = o._uids;
    _toggleCondition = o._toggleCondition;

    return *this;
}

void
RuleConditionNFC::_init() {
    connect(this, SIGNAL(uidsChanged()),         this, SIGNAL(changed()));
    connect(this, SIGNAL(toggleConditionChanged()), this, SIGNAL(changed()));
}


void
RuleConditionNFC::setUids(const QSet<QByteArray> &uids) {
    _uids = uids;
    emit uidsChanged();
}

// IMPROVE: same function exists in rulecondition.cpp
namespace {
bool variantQStringLessThan(const QVariant &s1, const QVariant &s2)
{
    return s1.toString() < s2.toString();
}
}

// For QML
QVariantList
RuleConditionNFC::getUidsQml() const {
    QVariantList uids;
    for (QSet<QByteArray>::const_iterator i = _uids.constBegin(); i != _uids.constEnd(); ++i) {
        QString uidHex(i->toHex());
        uids << uidHex; // QVariant::fromValue(*i);
    }
    qSort(uids.begin(), uids.end(), variantQStringLessThan);
    return uids;

}

void
RuleConditionNFC::setUidsQml(const QVariantList &uids) {
    qDebug("RuleConditionNFC::setUidsQml");
    QSet<QByteArray> uidsSet;
    for (QVariantList::const_iterator i = uids.constBegin(); i != uids.constEnd(); ++i) {
        uidsSet << QByteArray::fromHex(i->toByteArray());
        qDebug("RuleConditionNFC::setUidsQml: adding %s", qPrintable(i->toString()));
    }
    setUids(uidsSet);
}

void
RuleConditionNFC::setToggleCondition(bool toggle) {
    if (_toggleCondition != toggle) {
        _toggleCondition = toggle;
        emit toggleConditionChanged();
    }
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleConditionNFC &rule)
{
    argument.beginStructure();
    argument << rule.getUids();
    argument << rule.getToggleCondition();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleConditionNFC &rule)
{
    QList<QByteArray> uids;
    bool toggleCondition = false;

    argument.beginStructure();
    argument >> uids;
    argument >> toggleCondition;
    argument.endStructure();

    rule.setUids(QSet<QByteArray>::fromList(uids));
    rule.setToggleCondition(toggleCondition);

    return argument;
}
