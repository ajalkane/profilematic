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

#include "actionpresenceimpl.h"

#include <TelepathyQt4/PendingReady>
#include <Accounts/Manager>

ActionPresenceImpl::ActionPresenceImpl() :
    _pendingRule(NULL)
{
    Tp::registerTypes();

    _manager = new Accounts::Manager(this);
    _accountManager = Tp::AccountManager::create();
    connect(_accountManager->becomeReady(Tp::AccountManager::FeatureCore),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountManagerReady(Tp::PendingOperation*)));
}

void ActionPresenceImpl::activate(const Rule &rule)
{
    if (!_accountManager->isReady(Tp::AccountManager::FeatureCore)) {
        qDebug() << "Rule was activated while Telepathy Account Manager was not ready - will retry as soon as it is ready.";

        if (_pendingRule)
            delete _pendingRule;

        _pendingRule = new Rule(rule);
        _pendingRule->setParent(this);

        return;
    }

    bool hadPreviousPresences = !_previousPresences.isEmpty();

    // Restore previous presences if requested by the previous rule
    foreach (const AccountPresence &previousPresence, _previousPresences) {
        Tp::AccountPtr tpAccount
                = _accountManager->accountForPath(previousPresence.objectPath);

        if (!tpAccount)
            continue;

        changeAccountPresence(tpAccount, previousPresence.presence);
    }

    _previousPresences.clear();

    if (rule.getRestorePresence()) {
        foreach(const Tp::AccountPtr &account, _accountManager->allAccounts())
            _previousPresences.append(AccountPresence(account));
    }

    // In case we switch back to the default rule and previous availability
    // information was restored - account availabilities set by the default
    // rule should be ignored.
    if (rule.isDefaultRule() && hadPreviousPresences)
        return;


    // Handle the special cases of all online or all offline
    switch(rule.getPresenceChangeType()) {
    case Rule::AllOfflinePresenceType:
    case Rule::AllOnlinePresenceType:
        changeAllAccounts(rule);
        break;
    case Rule::CustomPresenceType:
        changeSelectedAccounts(rule);
        break;
    }
}

void ActionPresenceImpl::onPresenceChangeFinished(Tp::PendingOperation *op)
{
    if (op->isError())
        qWarning() << "Failed to change presence:" << op->errorMessage();
}

void ActionPresenceImpl::onAccountManagerReady(Tp::PendingOperation *op)
{
    if (op->isError()) {
        qWarning() << "Failed to instantiate account manager.";
        return;
    }

    if (!_pendingRule)
        return;

    activate(*_pendingRule);

    delete _pendingRule;
    _pendingRule = NULL;
}

void ActionPresenceImpl::changeAccountPresence(Tp::AccountPtr account, const Tp::Presence &presence)
{
    Tp::PendingOperation *op = account->setRequestedPresence(presence);
    connect(op,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onPresenceChangeFinished(Tp::PendingOperation*)));
}

void ActionPresenceImpl::changeAllAccounts(const Rule &rule)
{
    Tp::Presence presence
            = rule.getPresenceChangeType() == Rule::AllOfflinePresenceType ? Tp::Presence::offline() : Tp::Presence::available(rule.getPresenceStatusMessage());
    foreach(const Tp::AccountPtr &account, _accountManager->allAccounts()) {
        changeAccountPresence(account, presence);
    }
}

void ActionPresenceImpl::changeSelectedAccounts(const Rule &rule)
{
    foreach(const PresenceRule *presenceRule, rule.presenceRules()) {
        Accounts::Account *account
                = _manager->account(presenceRule->accountId());
        Accounts::Service *selectedService = NULL;

        foreach (Accounts::Service *service, account->services()) {
            if (service->name() == presenceRule->serviceName()) {
                selectedService = service;
                break;
            }
        }

        if (!selectedService) {
            qWarning() << "Could not find service" << presenceRule->serviceName();
            continue;
        }

        account->selectService(selectedService);

        QString uid = account->valueAsString("tmc-uid");
        if (uid.isEmpty()) {
            qWarning() << "Failed to retrieve tmc-uid for account" << presenceRule->accountId();
            continue;
        }

        Tp::AccountPtr tpAccount =
                _accountManager->accountForPath(QString("/org/freedesktop/Telepathy/Account/%1").arg(uid));

        if (!tpAccount) {
            qWarning() << "Failed to retrieve Telepathy account for" << presenceRule->accountId() << uid;
            continue;
        }

        Tp::Presence presence;
        QString statusMessage = presenceRule->statusMessage();

        if (statusMessage.isEmpty())
            statusMessage = rule.getPresenceStatusMessage();

        switch (presenceRule->action()) {
        case PresenceRule::SetOnline:
            presence = Tp::Presence::available(statusMessage);
            break;
        case PresenceRule::SetOffline:
            presence = Tp::Presence::offline();
            break;
        default:
            continue;
        }

        changeAccountPresence(tpAccount, presence);
    }
}

ActionPresenceImpl::AccountPresence::AccountPresence(const Tp::AccountPtr &account) :
    objectPath(account->objectPath()),
    presence(account->requestedPresence())
{
}
