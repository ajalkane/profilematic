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

    _networkConfigurationManager = new QNetworkConfigurationManager(this);
    _manager = new Accounts::Manager(this);
    _accountManager = Tp::AccountManager::create();
    connect(_accountManager->becomeReady(Tp::AccountManager::FeatureCore),
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onAccountManagerReady(Tp::PendingOperation*)));
}

bool
ActionPresenceImpl::_hasPresenceChanges(const RuleAction &ruleAction) {
    if (ruleAction.getPresenceChangeType() != RuleAction::CustomPresenceType) {
        return true;
    }

    bool noPresenceChanges = true;

    foreach (PresenceRule *presenceRule, ruleAction.presenceRules()) {
        // Skip rules which enforce the default action
        if (presenceRule->action() != PresenceRule::Retain)
            noPresenceChanges = false;
    }

    return !noPresenceChanges;
}

void
ActionPresenceImpl::_delayRuleActivation(const Rule::IdType &ruleId, const RuleAction &rule) {
    if (_pendingRule)
        delete _pendingRule;

    _pendingRule = new RuleAction(rule);
    _pendingRule->setParent(this);
    _pendingRuleId = ruleId;
}

void
ActionPresenceImpl::_activatePendingRule() {
    if (!_pendingRule) {
        qDebug("%s ActionPresence::_activatePendingRule() no pending rule", qPrintable(QDateTime::currentDateTime().toString()));
        return;
    }

    activateDifferent(_pendingRuleId, *_pendingRule);

    delete _pendingRule;
    _pendingRule = NULL;
    _pendingRuleId.clear();
}

bool
ActionPresenceImpl::activateDifferent(const Rule::IdType &ruleId, const RuleAction &rule)
{
    bool hasPresenceChanges = _hasPresenceChanges(rule);
    bool hadPreviousPresences = !_previousPresences.isEmpty();

    if (!hasPresenceChanges && !hadPreviousPresences) {
        qDebug() << "ActionPresence::activate rule has no presence changes or previous presences";
        return false;
    }

    if (!_accountManager->isReady(Tp::AccountManager::FeatureCore)) {
        qDebug() << "ActionPresence::activate Rule was activated while Telepathy Account Manager was not ready - will retry as soon as it is ready.";
        _delayRuleActivation(ruleId, rule);
        return true;
    }
    if (!_networkConfigurationManager->isOnline()) {
        qDebug() << "ActionPresence::activate Rule was activated while no connection - will retry as soon as it is ready.";
        _delayRuleActivation(ruleId, rule);
        connect(_networkConfigurationManager, SIGNAL(onlineStateChanged(bool)), this, SLOT(onOnlineStateChanged(bool)));
        return true;

    }
    if (useRestoreAction(ruleId, hasPresenceChanges, hadPreviousPresences)) {
        qDebug() << "ActionPresence::activate restoring previous presences";
        // Restore previous presences if requested by the previous rule
        foreach (const AccountPresence &previousPresence, _previousPresences) {
            Tp::AccountPtr tpAccount
                    = _accountManager->accountForPath(previousPresence.objectPath);

            if (!tpAccount)
                continue;

            changeAccountPresence(tpAccount, previousPresence.presence);
        }
        _previousPresences.clear();
        // Restore is not returned as activation
        return false;
    } else if (!hasPresenceChanges) {
        qDebug() << "ActionPresence::activate not setting presence";
        return false;
    }

    _previousPresences.clear();

    if (rule.getRestorePresence()) {
        foreach(Accounts::AccountId accountId, _manager->accountList()) {
            Accounts::Account *account = _manager->account(accountId);
            foreach (const Accounts::Service *service, account->services()) {
                account->selectService(service);

                QString uid = account->valueAsString("tmc-uid");

                if (uid.isEmpty())
                    continue;

                Tp::AccountPtr tpAccount
                        = _accountManager->accountForPath(QString("/org/freedesktop/Telepathy/Account/%1").arg(uid));

                if (!tpAccount) {
                    qWarning() << "ActionPresenceImpl::activate could not find Telepathy account for" << uid;
                    continue;
                }

                _previousPresences.append(AccountPresence(tpAccount));
            }
        }
    }

    changeAccountPresences(rule);

    return true;
}

void ActionPresenceImpl::onConnectsAutomaticallyChangeFinished(Tp::PendingOperation *op)
{
    if (op->isError())
        qWarning() << "ActionPresence::onConnectsAutomaticallyChangeFinished failed to change:" << op->errorMessage();
}

void ActionPresenceImpl::onAutomaticPresenceChangeFinished(Tp::PendingOperation *op)
{
    if (op->isError())
        qWarning() << "ActionPresence::onAutomaticPresenceChangeFinished failed to change:" << op->errorMessage();
}

void ActionPresenceImpl::onPresenceChangeFinished(Tp::PendingOperation *op)
{
    if (op->isError())
        qWarning() << "ActionPresence::onPresenceChangeFinished failed to change presence:" << op->errorMessage();
}

void ActionPresenceImpl::onCurrentPresenceChanged(const Tp::Presence &currentPresence) {
    qDebug() << "ActionPresenceImpl::onCurrentPresenceChanged called";

    Tp::AccountPtr account = Tp::AccountPtr(qobject_cast<Tp::Account *>(sender()));

    if (!account) {
        qWarning() << "ActionPresenceImpl::onCurrentPresenceChanged called from non Tp::Account object";
        return;
    }

    disconnect(account.data(), SIGNAL(currentPresenceChanged(Tp::Presence)),
               this, SLOT(onCurrentPresenceChanged(Tp::Presence)));

    Tp::Presence requestedPresence = _requestedPresences.take(account);

    if (!requestedPresence.isValid())
        return;

    if (currentPresence.type() == requestedPresence.type())
        return;

    qDebug() << "ActionPresenceImpl::onCurrentPresenceChanged setting actually requested presence.";
    Tp::PendingOperation *op = account->setRequestedPresence(requestedPresence);
    connect(op,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onPresenceChangeFinished(Tp::PendingOperation*)));
}

void ActionPresenceImpl::onAccountManagerReady(Tp::PendingOperation *op)
{
    qDebug("%s ActionPresence::onAccountManagerReady()", qPrintable(QDateTime::currentDateTime().toString()));

    if (op->isError()) {
        qWarning() << "ActionPresence::onAccountManagerReady failed to instantiate account manager.";
        return;
    }

    _activatePendingRule();
}

void
ActionPresenceImpl::onOnlineStateChanged(bool isOnline) {
    qDebug("%s ActionPresence::onOnlineStateChanged(%d)", qPrintable(QDateTime::currentDateTime().toString()), isOnline);
    if (isOnline) {
        disconnect(_networkConfigurationManager, SIGNAL(onlineStateChanged(bool)), this, SLOT(onOnlineStateChanged(bool)));
        _activatePendingRule();
    }
}

void ActionPresenceImpl::changeAccountPresence(Tp::AccountPtr account, const Tp::Presence &presence)
{
    qDebug() << "ActionPresenceImpl::changeAccountPresence: Changing account presence to" << presence.status() << "for" << account->uniqueIdentifier();
    Tp::PendingOperation *op;

    op = account->setConnectsAutomatically(presence.type() != Tp::ConnectionPresenceTypeOffline);
    connect(op,
            SIGNAL(finished(Tp::PendingOperation*)),
            SLOT(onConnectsAutomaticallyChangeFinished(Tp::PendingOperation*)));

    if (presence.type() != Tp::ConnectionPresenceTypeOffline) {
         op = account->setAutomaticPresence(presence);
         connect(op,
                 SIGNAL(finished(Tp::PendingOperation*)),
                 SLOT(onAutomaticPresenceChangeFinished(Tp::PendingOperation*)));
    }

    // Telepathy does not seem to adhere presence changes when the given account
    // is currently in connecting mode. Therefore, we save the requested
    // presence and apply it as soon as the current presence change was applied.
    if (account->connectionStatus() == Tp::ConnectionStatusConnecting) {
        _requestedPresences[account] = presence;
        connect(account.data(), SIGNAL(currentPresenceChanged(Tp::Presence)),
                SLOT(onCurrentPresenceChanged(Tp::Presence)));
    } else {
        op = account->setRequestedPresence(presence);
        connect(op,
                SIGNAL(finished(Tp::PendingOperation*)),
                SLOT(onPresenceChangeFinished(Tp::PendingOperation*)));
    }
}

void ActionPresenceImpl::changeAccountPresences(const RuleAction &rule)
{
    Tp::Presence defaultPresence;
    switch (rule.getPresenceChangeType()) {
    case RuleAction::AllOfflinePresenceType:
        defaultPresence = Tp::Presence::offline();
        break;
    case RuleAction::AllOnlinePresenceType:
        defaultPresence = Tp::Presence::available(rule.getPresenceStatusMessage());
        break;
    default:
        break;
    }

    foreach(Accounts::AccountId accountId, _manager->accountList()) {
        Accounts::Account *account = _manager->account(accountId);
        foreach (const Accounts::Service *service, account->services()) {
            account->selectService(service);

            QString uid = account->valueAsString("tmc-uid");

            if (uid.isEmpty())
                continue;

            Tp::AccountPtr tpAccount
                    = _accountManager->accountForPath(QString("/org/freedesktop/Telepathy/Account/%1").arg(uid));

            if (!tpAccount) {
                qWarning() << "ActionPresenceImpl::changeAccountPresences could not find Telepathy account for" << uid;
                continue;
            }

            Tp::Presence presence = defaultPresence;

            switch (rule.getPresenceChangeType()) {
            case RuleAction::AllOnlinePresenceType:
            case RuleAction::CustomPresenceType:
                presence = accountPresence(rule, account, service, defaultPresence);
                break;
            default:
                break;
            }

            if (!presence.isValid())
                continue;

            changeAccountPresence(tpAccount, presence);
        }
    }
}

Tp::Presence ActionPresenceImpl::accountPresence(const RuleAction &rule,
                                                 const Accounts::Account *account,
                                                 const Accounts::Service *service,
                                                 const Tp::Presence &defaultPresence) const
{
    foreach(const PresenceRule *presenceRule, rule.presenceRules()) {
        if (presenceRule->accountId() != account->id()
                || presenceRule->serviceName() != service->name())
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
