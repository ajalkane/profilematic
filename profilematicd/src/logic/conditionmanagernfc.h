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
#ifndef CONDITIONMANAGERNFC_H
#define CONDITIONMANAGERNFC_H

#include <QNearFieldManager>
#include <QSet>
#include <QHash>

QTM_USE_NAMESPACE

#include "conditionmanagercacheable.h"

class ConditionManagerNFC : public ConditionManagerCacheable {
    Q_OBJECT

    QNearFieldManager *_nfcManager;
    // QtMobility on N9 seems to have a bug (?), on targetLost the target->uid is empty.
    // So we have to cache the current nfc uid.
    QByteArray _currentNfcUid;
    QSet<Rule::IdType> _currentToggledRules;
    QHash<Rule::IdType, QSet<QByteArray> > _watchedTogglingNfcUidsByRuleId;
    QSet<QByteArray> _watchedNonTogglingNfcUids;
    QSet<QByteArray> _watchedTogglingNfcUids;

    void _clearVars();
    inline bool _conditionSetForMatching(const RuleCondition &cond) const { return !cond.nfc().getUids().isEmpty(); }

    void _monitorNfc(bool monitor);
public:
    ConditionManagerNFC(QObject *parent = 0);

    virtual bool conditionSetForMatching(const RuleCondition &cond) const;
    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond);

    virtual void startMonitor();
    virtual void stopMonitor();

    virtual void rulesChanged();

private slots:
    void targetDetected(QNearFieldTarget *target);
    void targetLost(QNearFieldTarget *target);
};

#endif // CONDITIONMANAGERNFC_H
