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

#include <QDomDocument>

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
    case SupportedPresencesRole: {
        QList<QVariant> supportedPresences;
        foreach (PresenceRule::Action presence, entry->supportedPresences)
            supportedPresences << int(presence);
        return supportedPresences;
    }
    default:
        return QVariant();
    }
}

void QmlPresenceModelImpl::onAccountCreated(Accounts::AccountId id)
{
    Accounts::Account *account = _manager->account(id);

#ifdef MEEGO_SUBEDITION_NEMO
    foreach(Accounts::Service serviceObj, account->services()) {
        Accounts::Service *service = &serviceObj;
#else
    foreach(Accounts::Service *service, account->services()) {
#endif
        if (!supportedService(account, service))
            continue;

        beginInsertRows(QModelIndex(), _rows.count(), _rows.count());
        _rows.append(new AccountsQtEntry(account,
                                         service,
                                         createPresenceRule(id, service->name())));
        endInsertRows();
    }
}

void QmlPresenceModelImpl::onAccountRemoved(Accounts::AccountId id)
{
    QMutableListIterator<AccountEntry *> it(_rows);
    for (int row = 0; row < _rows.count(); row++) {
        if (_rows[row]->rule->accountId() != id)
            continue;

        beginRemoveRows(QModelIndex(), row, row);
        _rows.removeAt(row);
        endRemoveRows();

        row--;
    }
}

void QmlPresenceModelImpl::buildAccountList()
{
    foreach(Accounts::AccountId accountId, _manager->accountList()) {
        Accounts::Account *account = _manager->account(accountId);

#ifdef MEEGO_SUBEDITION_NEMO
        foreach (Accounts::Service serviceObj, account->services("IM")) {
            Accounts::Service *service = &serviceObj;
#else
        foreach (Accounts::Service *service, account->services("IM")) {
#endif
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
#ifdef MEEGO_SUBEDITION_NEMO
    account->selectService(const_cast<const Accounts::Service &>(*service));
#else
    account->selectService(service);
#endif
    return !account->valueAsString("tmc-uid").isEmpty();
}

#include <QDebug>

QmlPresenceModelImpl::AccountsQtEntry::AccountsQtEntry(Accounts::Account *account,
                                                       Accounts::Service *service,
                                                       PresenceRule *presenceRule) :
    AccountEntry(presenceRule),
    account(account),
    service(service)
{
    QDomDocument dom = service->domDocument();
    QDomElement root = dom.documentElement();
    QDomElement typeData = root.firstChildElement("type-data");
    if (typeData.isNull())
        return;
    QDomElement servicePresentList = typeData.firstChildElement("service-presences-list");
    if (servicePresentList.isNull())
        return;
    QDomElement presence = servicePresentList.firstChildElement("presence");
    while (!presence.isNull()) {
        QString type = presence.attribute("type");

        if (type == "Offline")
            supportedPresences.append(PresenceRule::SetOffline);
        else if (type == "Available")
            supportedPresences.append(PresenceRule::SetOnline);
        else if (type == "Away")
            supportedPresences.append(PresenceRule::SetAway);
        else if (type == "Extended_Away")
            supportedPresences.append(PresenceRule::SetXa);
        else if (type == "Hidden")
            supportedPresences.append(PresenceRule::SetHidden);
        else if (type == "Busy")
            supportedPresences.append(PresenceRule::SetBusy);

        presence = presence.nextSiblingElement("presence");
    }

}
