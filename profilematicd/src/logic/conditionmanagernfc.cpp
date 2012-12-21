/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
 *
 * This file is part of ProfileMatic.
 *
 * ProfileMatic is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ProfileMatic is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProfileMatic.  If not, see <http://www.gnu.org/licenses/>
**/
#include <QDebug>
#include "conditionmanagernfc.h"

ConditionManagerNFC::ConditionManagerNFC(QObject *parent) : ConditionManagerCacheable(parent)
{
    setObjectName("ConditionManagerNFC");
    _nfcManager = new QNearFieldManager(this);

    qDebug("ConditionManagerNFC isAvailable: %d", _nfcManager->isAvailable());
    connect(_nfcManager, SIGNAL(targetDetected(QNearFieldTarget *)), this, SLOT(targetDetected(QNearFieldTarget *)));
    connect(_nfcManager, SIGNAL(targetLost(QNearFieldTarget *)), this, SLOT(targetLost(QNearFieldTarget *)));
}

bool
ConditionManagerNFC::conditionSetForMatching(const RuleCondition &cond) const {
    return _conditionSetForMatching(cond);
}

ConditionManagerCacheable::MatchStatus
ConditionManagerNFC::match(const Rule::IdType &ruleId, const RuleCondition &cond) {
    if (!_conditionSetForMatching(cond)) {
        qDebug() << "ConditionManagerNFC::match() options not set or invalid, matchNotSet";
        return MatchNotSet;
    }
    const QSet<QByteArray> &nfcUids = cond.nfc().getUids();
    if (nfcUids.isEmpty()) {
        qDebug("ConditionManagerNFC::match nfcUids is empty, matches");
        return Matched;

    }

    bool matches = false;
    if (cond.nfc().getToggleCondition()) {
        qDebug("ConditionManagerNFC::match adding to toggling nfcUids %d ids", nfcUids.size());
        matches = _currentToggledRules.contains(ruleId);
        _watchedTogglingNfcUids.unite(nfcUids);
        _watchedTogglingNfcUidsByRuleId[ruleId] = nfcUids;
    } else {
        qDebug("ConditionManagerNFC::match adding to non-toggling nfcUids %d ids", nfcUids.size());
        matches = nfcUids.contains(_currentNfcUid);
        _watchedNonTogglingNfcUids.unite(nfcUids);
    }

    qDebug("ConditionManagerNFC::match matches %d", matches);

    return matches ? Matched : NotMatched;
}

void
ConditionManagerNFC::startMonitor() {
    qDebug() << "ConditionManagerNFC::startMonitor";
    _nfcManager->startTargetDetection();
}

void
ConditionManagerNFC::stopMonitor() {
    qDebug() << "ConditionManagerNFC::stopMonitor";
    _nfcManager->stopTargetDetection();

    _currentNfcUid.clear();
    _currentToggledRules.clear();
    _clearVars();
}

void
ConditionManagerNFC::rulesChanged() {
    qDebug() << "ConditionManagerNFC::rulesChanged";

    _clearVars();
}

void
ConditionManagerNFC::targetDetected(QNearFieldTarget *target) {
    qDebug("%s ConditionManagerNFC::targetDetected()", qPrintable(QDateTime::currentDateTime().toString()));
    QByteArray uid = target->uid();
    _currentNfcUid = uid;
    QString uidStr(uid.toHex());
    bool doInvalidate = false;
    qDebug("ConditionManagerNFC::targetDetected() uid: %s", qPrintable(uidStr));
    if (_watchedTogglingNfcUids.contains(uid)) {        
        qDebug("ConditionManagerNFC::targetDetected() toggling NFC, invalidating");
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
                doInvalidate  = true;
            }
        }
    }
    if (_watchedNonTogglingNfcUids.contains(uid)) {
        qDebug("ConditionManagerNFC::targetDetected() non-toggling NFC, invalidating");
        doInvalidate  = true;
    }

    if (doInvalidate ) {
        _clearVars();
        emit matchInvalidated();
    }
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
        _clearVars();
        emit matchInvalidated();
    }
}

void
ConditionManagerNFC::_clearVars() {
    _watchedNonTogglingNfcUids.clear();
    _watchedTogglingNfcUids.clear();
    _watchedTogglingNfcUidsByRuleId.clear();
}
