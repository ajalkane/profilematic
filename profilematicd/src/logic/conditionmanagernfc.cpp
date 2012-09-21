#include "conditionmanagernfc.h"

ConditionManagerNFC::ConditionManagerNFC()
{
    _nfcManager = new QNearFieldManager(this);

    qDebug("ConditionManagerNFC isAvailable: %d", _nfcManager->isAvailable());
    connect(_nfcManager, SIGNAL(targetDetected(QNearFieldTarget *)), this, SLOT(targetDetected(QNearFieldTarget *)));
    connect(_nfcManager, SIGNAL(targetLost(QNearFieldTarget *)), this, SLOT(targetLost(QNearFieldTarget *)));
}

void
ConditionManagerNFC::startRefresh() {
    _watchedNonTogglingNfcUids.clear();
    _watchedTogglingNfcUids.clear();
    _watchedTogglingNfcUidsByRuleId.clear();
}

bool
ConditionManagerNFC::refresh(const Rule::IdType &ruleId, const RuleCondition &rule) {
    const QSet<QByteArray> &nfcUids = rule.nfc().getUids();
    if (nfcUids.isEmpty()) {
        qDebug("ConditionManagerNFC::refresh nfcUids is empty, matches");
        return true;

    }

    bool matches = false;
    if (rule.nfc().getToggleCondition()) {
        qDebug("ConditionManagerNFC::refresh adding to toggling nfcUids %d ids", nfcUids.size());
        matches = _currentToggledRules.contains(ruleId);
        _watchedTogglingNfcUids.unite(nfcUids);
        _watchedTogglingNfcUidsByRuleId[ruleId] = nfcUids;
    } else {
        qDebug("ConditionManagerNFC::refresh adding to non-toggling nfcUids %d ids", nfcUids.size());
        matches = nfcUids.contains(_currentNfcUid);
        _watchedNonTogglingNfcUids.unite(nfcUids);
    }

    qDebug("ConditionManagerNFC::refresh matches %d", matches);

    return matches;
}

void
ConditionManagerNFC::matchedRule(const RuleCondition &/*rule*/) {
}

void
ConditionManagerNFC::endRefresh() {
    if (!_watchedNonTogglingNfcUids.isEmpty() || !_watchedTogglingNfcUids.isEmpty()) {
        _monitorNfc(true);
    } else {
        _monitorNfc(false);
        _currentNfcUid.clear();
        _currentToggledRules.clear();
        _watchedTogglingNfcUids.clear();
        _watchedTogglingNfcUidsByRuleId.clear();
        _watchedNonTogglingNfcUids.clear();
    }
}

void
ConditionManagerNFC::_monitorNfc(bool monitor) {
    if (monitor) {
        _nfcManager->startTargetDetection();
    } else {
        _nfcManager->stopTargetDetection();
    }
}

void
ConditionManagerNFC::targetDetected(QNearFieldTarget *target) {
    qDebug("%s ConditionManagerNFC::targetDetected()", qPrintable(QDateTime::currentDateTime().toString()));
    QByteArray uid = target->uid();
    _currentNfcUid = uid;
    QString uidStr(uid.toHex());
    bool doRefresh = false;
    qDebug("ConditionManagerNFC::targetDetected() uid: %s", qPrintable(uidStr));
    if (_watchedTogglingNfcUids.contains(uid)) {        
        qDebug("ConditionManagerNFC::targetDetected() toggling NFC, refresh");
        QHash<Rule::IdType, QSet<QByteArray> >::const_iterator i = _watchedTogglingNfcUidsByRuleId.constBegin();
        for (; i != _watchedTogglingNfcUidsByRuleId.constEnd(); ++i) {
            const QSet<QByteArray> &uids = i.value();
            if (uids.contains(uid)) {
                const Rule::IdType ruleId = i.key();
                if (_currentToggledRules.contains(ruleId)) {
                    qDebug("ConditionManagerNFC::targetDetected() toggling off rule %s", qPrintable(ruleId));
                    _currentToggledRules.remove(ruleId);
                } else {
                    qDebug("ConditionManagerNFC::targetDetected() toggling on rule %s", qPrintable(ruleId));
                    _currentToggledRules.insert(ruleId);
                }
                doRefresh = true;
            }
        }
    }
    if (_watchedNonTogglingNfcUids.contains(uid)) {
        qDebug("ConditionManagerNFC::targetDetected() non-toggling NFC, refresh");
        doRefresh = true;
    }

    if (doRefresh) emit refreshNeeded();
}

void
ConditionManagerNFC::targetLost(QNearFieldTarget */*target*/) {
    qDebug("%s ConditionManagerNFC::targetLostDetected()", qPrintable(QDateTime::currentDateTime().toString()));
    // QtMobility on N9 seems to have a bug (?), target->uid is empty
    QByteArray uid = _currentNfcUid; // target->uid();
    QString uidStr(uid.toHex());
    _currentNfcUid.clear();
    qDebug("ConditionManagerNFC::targetLostDetected() uid: %s", qPrintable(uidStr));

    if (_watchedNonTogglingNfcUids.contains(uid)) {
        qDebug("ConditionManagerNFC::targetLostDetected() was in current non-toggling ids, refresh");
        emit refreshNeeded();
    }
}
