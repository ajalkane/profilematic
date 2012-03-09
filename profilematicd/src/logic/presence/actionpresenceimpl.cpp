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
        qDebug() << "ActionPresence::activate Rule was activated while Telepathy Account Manager was not ready - will retry as soon as it is ready.";

        if (_pendingRule)
            delete _pendingRule;

        _pendingRule = new Rule(rule);
        _pendingRule->setParent(this);

        return;
    }

    bool hadPreviousPresences = !_previousPresences.isEmpty();

    if (hadPreviousPresences) {
        qDebug() << "ActionPresence::activate restoring previous presences";
    }

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
    if (rule.isDefaultRule() && hadPreviousPresences) {
        qDebug() << "ActionPresence::activate restored previous preferences. Current rule is default rule - overriding its settings";
        return;
    }

    changeAccountPresences(rule);
}

void ActionPresenceImpl::onPresenceChangeFinished(Tp::PendingOperation *op)
{
    if (op->isError())
        qWarning() << "ActionPresence::onPresenceChangeFinished failed to change presence:" << op->errorMessage();
}

void ActionPresenceImpl::onAccountManagerReady(Tp::PendingOperation *op)
{
    qDebug("%s ActionPresence::onAccountManagerReady()", qPrintable(QDateTime::currentDateTime().toString()));

    if (op->isError()) {
        qWarning() << "ActionPresence::onAccountManagerReady failed to instantiate account manager.";
        return;
    }

    if (!_pendingRule) {
        qDebug("%s ActionPresence::onAccountManagerReady() no pending rule", qPrintable(QDateTime::currentDateTime().toString()));
        return;
    }

    activate(*_pendingRule);

    delete _pendingRule;
    _pendingRule = NULL;
}

void ActionPresenceImpl::changeAccountPresence(Tp::AccountPtr account, const Tp::Presence &presence)
{
    qDebug() << "ActionPresenceImpl::changeAccountPresence: Changing account presence to" << presence.status() << "for" << account->uniqueIdentifier();
    Tp::PendingOperation *op = account->setRequestedPresence(presence);
    connect(op,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onPresenceChangeFinished(Tp::PendingOperation*)));
}

void ActionPresenceImpl::changeAccountPresences(const Rule &rule)
{
    Tp::Presence defaultPresence;
    switch (rule.getPresenceChangeType()) {
    case Rule::AllOfflinePresenceType:
        defaultPresence = Tp::Presence::offline();
        break;
    case Rule::AllOnlinePresenceType:
        defaultPresence = Tp::Presence::available(rule.getPresenceStatusMessage());
        break;
    default:
        break;
    }

    foreach(const Tp::AccountPtr &account, _accountManager->allAccounts()) {
        Tp::Presence presence = defaultPresence;

        switch (rule.getPresenceChangeType()) {
        case Rule::AllOnlinePresenceType:
        case Rule::CustomPresenceType:
            presence = accountPresence(account, rule, defaultPresence);
            break;
        default:
            break;
        }

        if (!presence.isValid())
            continue;

        changeAccountPresence(account, presence);
    }
}

Tp::Presence ActionPresenceImpl::accountPresence(const Tp::AccountPtr &targetAccount,
                                                 const Rule &rule,
                                                 const Tp::Presence &defaultPresence) const
{
    foreach(const PresenceRule *presenceRule, rule.presenceRules()) {
        Accounts::Account *account
                = _manager->account(presenceRule->accountId());
        Accounts::Service *selectedService = NULL;

        if (account == NULL)
            continue;

        foreach (Accounts::Service *service, account->services()) {
            if (service->name() == presenceRule->serviceName()) {
                selectedService = service;
                break;
            }
        }

        if (!selectedService) {
            qWarning() << "ActionPresence::changeSelectedAccounts could not find service" << presenceRule->serviceName();
            continue;
        }

        account->selectService(selectedService);

        QString uid = account->valueAsString("tmc-uid");
        if (uid.isEmpty()) {
            qWarning() << "ActionPresence::changeSelectedAccounts failed to retrieve tmc-uid for account" << presenceRule->accountId();
            continue;
        }

        // Look-up the account using the path - Tp::Account only has uniqueId()
        // which only guarantees that the id is unique based on the current
        // D-Bus connection and therefore it must not necessarily match tmc-id.
        Tp::AccountPtr tpAccount =
                _accountManager->accountForPath(QString("/org/freedesktop/Telepathy/Account/%1").arg(uid));

        if (!tpAccount) {
            qWarning() << "ActionPresence::changeSelectedAccounts failed to retrieve Telepathy account for" << presenceRule->accountId() << uid;
            continue;
        }

        if (tpAccount->uniqueIdentifier() != targetAccount->uniqueIdentifier())
            continue;

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
        case PresenceRule::SetAway:
            presence = Tp::Presence::away(statusMessage);
            break;
        case PresenceRule::SetBrb:
            presence = Tp::Presence::brb(statusMessage);
            break;
        case PresenceRule::SetBusy:
            presence = Tp::Presence::busy(statusMessage);
            break;
        case PresenceRule::SetXa:
            presence = Tp::Presence::xa(statusMessage);
            break;
        case PresenceRule::SetHidden:
            presence = Tp::Presence::hidden(statusMessage);
            break;
        case PresenceRule::Retain:
            // Return invalid Tp::Presence
            break;
        }

        return presence;
    }

    return defaultPresence;
}

ActionPresenceImpl::AccountPresence::AccountPresence(const Tp::AccountPtr &account) :
    objectPath(account->objectPath()),
    presence(account->requestedPresence())
{
}
