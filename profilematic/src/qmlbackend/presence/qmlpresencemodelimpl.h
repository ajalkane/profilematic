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

#ifndef QMLACCOUNTSMODELIMPL_H
#define QMLACCOUNTSMODELIMPL_H

#include "qmlpresencemodel.h"

#include <Accounts/Manager>

class PresenceRule;

class QmlPresenceModelImpl : public QmlPresenceModel
{
    Q_OBJECT
public:
    explicit QmlPresenceModelImpl(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
protected:
    struct AccountsQtEntry : public AccountEntry {
        AccountsQtEntry(Accounts::Account *account,
                        Accounts::Service *service,
                        PresenceRule *presenceRule);
        Accounts::Account *account;
        Accounts::Service *service;
        QList<PresenceRule::Action> supportedPresences;
    };

private slots:
    void onAccountCreated(Accounts::AccountId);
    void onAccountRemoved(Accounts::AccountId);
private:
    void buildAccountList();
    bool supportedService(Accounts::Account *account, Accounts::Service *service);
private:
    Accounts::Manager *_manager;
};

#endif // QMLACCOUNTSMODELIMPL_H
