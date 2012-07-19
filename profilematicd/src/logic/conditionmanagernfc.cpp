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
}

bool
ConditionManagerNFC::refresh(const RuleCondition &rule) {
    const QSet<QByteArray> &nfcUids = rule.nfc().getUids();
    if (nfcUids.isEmpty()) {
        qDebug("ConditionManagerNFC::refresh nfcUids is empty, matches");
        return true;

    }

    QSet<QByteArray> *watchedNfcUids = 0;
    QSet<QByteArray> *currentNfcUids = 0;

    if (rule.nfc().getToggleCondition()) {
        qDebug("ConditionManagerNFC::refresh adding to toggling nfcUids %d ids", nfcUids.size());
        watchedNfcUids = &_watchedTogglingNfcUids;
        currentNfcUids = &_currentTogglingNfcUids;
    } else {
        qDebug("ConditionManagerNFC::refresh adding to non-toggling nfcUids %d ids", nfcUids.size());
        watchedNfcUids = &_watchedNonTogglingNfcUids;
        currentNfcUids = &_currentNonTogglingNfcUids;
    }
    watchedNfcUids->unite(nfcUids);

    qDebug("ConditionManagerNFC::refresh currentNfcUids %d", currentNfcUids->size());

    foreach (QByteArray nfcUid, *currentNfcUids) {
        if (nfcUids.contains(nfcUid)) {
            qDebug("ConditionManagerNFC::refresh contains one of currentNfcUids");
            return true;
        }
    }

    return false;
}

void
ConditionManagerNFC::matchedRule(const RuleCondition &/*rule*/) {
//    _currentRuleCellIds.unite(rule.getLocationCells());
//    _currentTimeout = qMax(_currentTimeout, rule.getLocationCellsTimeout());
}

void
ConditionManagerNFC::endRefresh() {
    if (!_watchedNonTogglingNfcUids.isEmpty() || !_watchedTogglingNfcUids.isEmpty()) {
        _monitorNfc(true);
    } else {
        _monitorNfc(false);
        // _currentRuleCellIds.clear();
        _currentTogglingNfcUids.clear();
        _currentNonTogglingNfcUids.clear();
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
    qDebug("ConditionManagerNFC::targetDetected()");
    QByteArray uid = target->uid();
    _currentNfcUid = uid;
    QString uidStr(uid.toHex());
    bool doRefresh = false;
    qDebug("ConditionManagerNFC::targetDetected() uid: %s", qPrintable(uidStr));
    if (_watchedTogglingNfcUids.contains(uid)) {
        if (_currentTogglingNfcUids.contains(uid)) {
            qDebug("ConditionManagerNFC::targetDetected() toggling off, refresh");
            _currentTogglingNfcUids.remove(uid);
        } else {
            qDebug("ConditionManagerNFC::targetDetected() toggling on, refresh");
            _currentTogglingNfcUids << uid;
        }
        doRefresh = true;
    }
    if (_watchedNonTogglingNfcUids.contains(uid)) {
        qDebug("ConditionManagerNFC::targetDetected() non-toggling NFC, refresh");
        _currentNonTogglingNfcUids << uid;
        doRefresh = true;
    }

    if (doRefresh) emit refreshNeeded();
}

void
ConditionManagerNFC::targetLost(QNearFieldTarget */*target*/) {
    qDebug("ConditionManagerNFC::targetLostDetected()");
    // QtMobility on N9 seems to have a bug (?), target->uid is empty
    QByteArray uid = _currentNfcUid; // target->uid();
    QString uidStr(uid.toHex());
    qDebug("ConditionManagerNFC::targetLostDetected() uid: %s", qPrintable(uidStr));

    if (_watchedNonTogglingNfcUids.contains(uid)) {
        qDebug("ConditionManagerNFC::targetLostDetected() was in current non-toggling ids, refresh");
        _currentNonTogglingNfcUids.remove(uid);
        emit refreshNeeded();
    }
}
