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

#include "presencerule.h"

PresenceRule::PresenceRule(const PresenceRule &presenceRule, QObject *parent) :
    QObject(parent),
    _key(presenceRule._key),
    _action(presenceRule._action),
    _statusMessage(presenceRule._statusMessage)
{
}

PresenceRule::PresenceRule(QObject *parent) :
    QObject(parent)
{
}

PresenceRule::PresenceRule(const Accounts::AccountId &accountId,
                           const QString &serviceName,
                           PresenceRule::Action action,
                           const QString &statusMessage,
                           QObject *parent) :
    QObject(parent),
    _key(accountId, serviceName),
    _action(action),
    _statusMessage(statusMessage)
{
}

Accounts::AccountId PresenceRule::accountId() const
{
    return _key.accountId;
}

const QString &PresenceRule::serviceName() const
{
    return _key.serviceName;
}

PresenceRule::Action PresenceRule::action() const
{
    return _action;
}

void PresenceRule::setAction(const PresenceRule::Action action)
{
    if (action == _action)
        return;

    _action = action;

    emit actionChanged();
}

const QString &PresenceRule::statusMessage() const
{
    return _statusMessage;
}

void PresenceRule::setStatusMessage(const QString &statusMessage)
{
    if (_statusMessage == statusMessage)
        return;

    _statusMessage = statusMessage;

    emit statusMessageChanged();
}

const PresenceRule::Key &PresenceRule::key() const
{
    return _key;
}

PresenceRule &PresenceRule::operator =(const PresenceRule &other)
{
    _key = other._key;
    _action = other._action;
    _statusMessage = other._statusMessage;

    return *this;
}

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const PresenceRule &obj) {
    dbusArgument.beginStructure();
    dbusArgument << obj.accountId();
    dbusArgument << obj.serviceName();
    dbusArgument << int(obj.action());
    dbusArgument << obj.statusMessage();
    dbusArgument.endStructure();

    return dbusArgument;
}

const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, PresenceRule &obj) {
    int action;

    dbusArgument.beginStructure();
    dbusArgument >> obj._key.accountId;
    dbusArgument >> obj._key.serviceName;
    dbusArgument >> action;
    dbusArgument >> obj._statusMessage;
    dbusArgument.endStructure();

    obj._action = (PresenceRule::Action) action;

    return dbusArgument;
}

PresenceRule::Key::Key()
{
}

PresenceRule::Key::Key(const Accounts::AccountId &accountId,
                       const QString &serviceName) :
    accountId(accountId),
    serviceName(serviceName)
{
}

bool PresenceRule::Key::operator ==(const PresenceRule::Key &other) const
{
    return accountId == other.accountId && serviceName == other.serviceName;
}
