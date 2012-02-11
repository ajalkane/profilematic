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

#include "qmlpresencemodelimpl.h"
#include "../../profilematicd/src/model/rule.h"
#include "../../profilematicd/src/model/presencerule.h"

QmlPresenceModelImpl::QmlPresenceModelImpl(QObject *parent) :
    QmlPresenceModel(parent)
{
    _manager = new Accounts::Manager(this);

    buildAccountList();

    connect(_manager,
            SIGNAL(accountCreated(Accounts::AccountId)),
            SLOT(onAccountCreated(Accounts::AccountId)));
    connect(_manager,
            SIGNAL(accountRemoved(Accounts::AccountId)),
            SLOT(onAccountRemoved(Accounts::AccountId)));
}

QVariant QmlPresenceModelImpl::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _rows.count())
        return QVariant();

    const AccountsQtEntry *entry
            = static_cast<const AccountsQtEntry *>(_rows[index.row()]);
    const Accounts::Account *account = entry->account;

    switch (role) {
    case AccountNameRole:
        return account->displayName();
    case AccountIdRole:
        return account->id();
    case AccountEnabledRole:
        return account->enabled();
    case PresenceRuleRole:
        return QVariant::fromValue(entry->rule);
    case AccountIconRole:
        return entry->service->iconName();
    default:
        return QVariant();
    }
}

void QmlPresenceModelImpl::onAccountCreated(Accounts::AccountId id)
{
    Accounts::Account *account = _manager->account(id);

    foreach(Accounts::Service *service, account->services()) {
        if (!supportedService(account, service))
            continue;

        beginInsertRows(QModelIndex(), _rows.count() - 1, _rows.count() - 1);
        _rows.append(new AccountsQtEntry(account,
                                         service,
                                         createPresenceRule(id, service->name())));
        endInsertRows();
    }
}

void QmlPresenceModelImpl::onAccountRemoved(Accounts::AccountId id)
{
    QMutableListIterator<AccountEntry *> it(_rows);
    int row = 0;
    while (it.hasNext()) {
        if ((it.value()->rule)->accountId() == id) {
            beginRemoveRows(QModelIndex(), row, row);
            it.remove();
            endRemoveRows();
            return;
        }

        row++;
        it.next();
    }
}

void QmlPresenceModelImpl::buildAccountList()
{
    foreach(Accounts::AccountId accountId, _manager->accountList()) {
        Accounts::Account *account = _manager->account(accountId);

        foreach (Accounts::Service *service, account->services("IM")) {
            // Determine whether it is a Telepathy based account if not skip it as
            // its presence cannot be changed.
            if (!supportedService(account, service))
                continue;

            _rows.append(new AccountsQtEntry(account,
                                             service,
                                             createPresenceRule(accountId, service->name())));
        }
    }
}

bool QmlPresenceModelImpl::supportedService(Accounts::Account *account,
                                            Accounts::Service *service)
{
    account->selectService(service);
    return !account->valueAsString("tmc-uid").isEmpty();
}

QmlPresenceModelImpl::AccountsQtEntry::AccountsQtEntry(Accounts::Account *account,
                                                       Accounts::Service *service,
                                                       PresenceRule *presenceRule) :
    AccountEntry(presenceRule),
    account(account),
    service(service)
{
}
