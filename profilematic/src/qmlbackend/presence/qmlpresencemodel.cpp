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

#include "model/ruleaction.h"

#include "qmlpresencemodel.h"

QmlPresenceModel::QmlPresenceModel(QObject *parent) :
    QAbstractListModel(parent), _action(NULL)
{
    QHash<int, QByteArray> roleMap;
    roleMap[AccountNameRole] = "accountName";
    roleMap[AccountIdRole] = "accountId";
    roleMap[AccountEnabledRole] = "accountEnabled";
    roleMap[PresenceRuleRole] = "presenceRule";
    roleMap[AccountIconRole] = "accountIcon";
    roleMap[SupportedPresencesRole] = "supportedPresences";
    setRoleNames(roleMap);

    connect(this,
            SIGNAL(rowsInserted(QModelIndex, int, int)),
            SLOT(onRowsInserted(QModelIndex, int, int)));
    connect(this,
            SIGNAL(rowsAboutToBeRemoved(QModelIndex, int, int)),
            SLOT(onRowsAboutToBeRemoved(QModelIndex, int, int)));

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
    PresenceRule::Action action = PresenceRule::Retain;

    if (_action) {
        switch (_action->getPresenceChangeType()) {
        case RuleAction::AllOfflinePresenceType:
            action = PresenceRule::SetOffline;
            break;
        case RuleAction::AllOnlinePresenceType:
            action = PresenceRule::SetOnline;
            break;
        default:
            break;
        }
    }

    PresenceRule *presenceRule
            = new PresenceRule(accountId, serviceName, action, QString(), this);
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

void QmlPresenceModel::onRowsAboutToBeRemoved(const QModelIndex &parent,
                                              int start,
                                              int end)
{
    if (parent.isValid() || !_action)
        return;

    for (int row = start; row <= end; row++) {
        const AccountEntry *entry = _rows[row];

        _action->removePresenceRule(entry->rule);
    }
}

void QmlPresenceModel::onRowsInserted(const QModelIndex &parent,
                                      int start,
                                      int end)
{
    if (parent.isValid() || !_action)
        return;

    for (int row = start; row <= end; row++) {
        AccountEntry *entry = _rows[row];

        _action->addPresenceRule(entry->rule);
    }
}

void QmlPresenceModel::updatePresenceRules()
{
    if (_action == NULL) {
        foreach (AccountEntry *entry, _rows) {
            // Create a new PresenceRule for each entry - the current rules
            // cannot be owned by this model as a previous rule must have been
            // set if we end up here.
            entry->rule = new PresenceRule(*entry->rule, this);
        }
        return;
    }

    // Add new accounts to configuration
    for (QList<AccountEntry *>::iterator it = _rows.begin();
         it != _rows.end();
         it++) {
        AccountEntry *entry = *it;

        PresenceRule *rule = _action->presenceRule(entry->rule->key());

        if (rule == entry->rule)
            continue;

        if (!rule) {
            rule = createPresenceRule(entry->rule->accountId(), entry->rule->serviceName());
            _action->addPresenceRule(rule);
        }

        if (entry->rule->parent() == this)
            entry->rule->deleteLater();

        entry->rule->disconnect(this);
        entry->rule = rule;
        connect(entry->rule, SIGNAL(actionChanged()), SLOT(onActionChanged()));
    }

    // Remove old accounts from configuration
    foreach (PresenceRule *presenceRule, _action->presenceRules()) {
        bool found = false;

        foreach (const AccountEntry *entry, _rows) {
            if (entry->rule->key() == presenceRule->key()) {
                found = true;
                break;
            }
        }

        if (found)
            continue;

        _action->removePresenceRule(presenceRule);
    }
}

RuleAction *QmlPresenceModel::action() const
{
    return _action;
}

void QmlPresenceModel::setAction(RuleAction *action)
{
    if (action == _action)
        return;

    if (_action)
        _action->disconnect(this);

    _action = action;

    updatePresenceRules();

    emit actionChanged();
}



QmlPresenceModel::AccountEntry::AccountEntry(PresenceRule *rule) :
    rule(rule)
{
}
