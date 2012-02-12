/**********************************************************************
 * Copyright 2011-2012 Philip Lorenz
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

#include "qmlpresencemodel.h"
#include "../../profilematicd/src/model/rule.h"

QmlPresenceModel::QmlPresenceModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QHash<int, QByteArray> roleMap;
    roleMap[AccountNameRole] = "accountName";
    roleMap[AccountIdRole] = "accountId";
    roleMap[AccountEnabledRole] = "accountEnabled";
    roleMap[PresenceRuleRole] = "presenceRule";
    roleMap[AccountIconRole] = "accountIcon";
    setRoleNames(roleMap);
}

QmlPresenceModel::~QmlPresenceModel()
{
    qDeleteAll(_rows);
    _rows.clear();
}

int QmlPresenceModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return _rows.count();
}

PresenceRule *QmlPresenceModel::createPresenceRule(const Accounts::AccountId &accountId,
                                                   const QString &serviceName)
{
    PresenceRule *presenceRule
            = new PresenceRule(accountId, serviceName, PresenceRule::Retain, QString(), this);
    connect(presenceRule, SIGNAL(actionChanged()), SLOT(onActionChanged()));
    return presenceRule;
}


void QmlPresenceModel::onActionChanged()
{
    PresenceRule *presenceRule = qobject_cast<PresenceRule *>(sender());

    if (!presenceRule)
        return;

    for (int row = 0; row < _rows.count(); row++) {
        if (_rows[row]->rule == presenceRule) {
            QModelIndex index = createIndex(row, 0);
            emit dataChanged(index, index);
            return;
        }
    }
}

QList<QObject *> QmlPresenceModel::presenceRules() const
{
    QList<QObject *> result;
    foreach (const AccountEntry *account, _rows) {
        result.append(account->rule);
    }
    return result;
}

void QmlPresenceModel::setPresenceRules(QList<QObject *> presenceRules)
{
    foreach (QObject *obj, presenceRules) {
        PresenceRule *presenceRule = qobject_cast<PresenceRule *>(obj);
        if (!presenceRule)
            continue;

        for (QList<AccountEntry *>::iterator it = _rows.begin();
             it != _rows.end();
             it++) {
            AccountEntry *entry = *it;
            if (entry->rule->accountId() != presenceRule->accountId())
                continue;

            entry->rule->setAction(presenceRule->action());
            break;
        }
    }

    emit presenceRulesChanged();
}



QmlPresenceModel::AccountEntry::AccountEntry(PresenceRule *rule) :
    rule(rule)
{
}
