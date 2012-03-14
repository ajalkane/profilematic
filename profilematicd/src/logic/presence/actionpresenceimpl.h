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

#ifndef ACTIONPRESENCEIMPL_H
#define ACTIONPRESENCEIMPL_H

#include "actionpresence.h"

#include <TelepathyQt4/AccountManager>

namespace Accounts {
class Manager;
}

class ActionPresenceImpl : public QObject, public ActionPresence
{
    Q_OBJECT
public:
    ActionPresenceImpl();

    void activate(const Rule &rule);
private slots:
    void onConnectsAutomaticallyChangeFinished(Tp::PendingOperation *op);
    void onAutomaticPresenceChangeFinished(Tp::PendingOperation *op);
    void onPresenceChangeFinished(Tp::PendingOperation *op);
    void onCurrentPresenceChanged(const Tp::Presence &currentPresence);
    void onAccountManagerReady(Tp::PendingOperation *op);
private:
    struct AccountPresence {
        AccountPresence(const Tp::AccountPtr &account);
        QString objectPath;
        Tp::Presence presence;
    };
private:
    void changeAccountPresence(Tp::AccountPtr account, const Tp::Presence &presence);
    void changeAccountPresences(const Rule &rule);
    Tp::Presence accountPresence(const Rule &rule,
                                 const Accounts::Account *account,
                                 const Accounts::Service *service,
                                 const Tp::Presence &defaultPresence) const;
private:
    Tp::AccountManagerPtr _accountManager;
    Accounts::Manager *_manager;

    Rule *_pendingRule;

    QList<AccountPresence> _previousPresences;
    QHash<Tp::AccountPtr, Tp::Presence> _requestedPresences;
};

#endif // ACTIONPRESENCEIMPL_H
