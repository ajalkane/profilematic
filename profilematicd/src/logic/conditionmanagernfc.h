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

QTM_USE_NAMESPACE

#include "conditionmanager.h"

class ConditionManagerNFC : public ConditionManager {
    Q_OBJECT

    QNearFieldManager *_nfcManager;
    // QtMobility on N9 seems to have a bug (?), on targetLost the target->uid is empty.
    // So we have to cache the current nfc uid.
    QByteArray _currentNfcUid;
    QSet<QByteArray> _currentTogglingNfcUids;
    QSet<QByteArray> _currentNonTogglingNfcUids;
    QSet<QByteArray> _watchedNonTogglingNfcUids;
    QSet<QByteArray> _watchedTogglingNfcUids;

    void _monitorNfc(bool monitor);
public:
    ConditionManagerNFC();

    virtual void startRefresh();
    virtual bool refresh(const RuleCondition &rule);
    virtual void matchedRule(const RuleCondition &rule);
    virtual void endRefresh();

private slots:
    void targetDetected(QNearFieldTarget *target);
    void targetLost(QNearFieldTarget *target);
};

#endif // CONDITIONMANAGERNFC_H
