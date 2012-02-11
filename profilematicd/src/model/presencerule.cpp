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
    _id(presenceRule._id),
    _serviceName(presenceRule._serviceName),
    _action(presenceRule._action)
{
}

PresenceRule::PresenceRule(QObject *parent) :
    QObject(parent)
{
}

PresenceRule::PresenceRule(const Accounts::AccountId &accountId,
                         const QString &serviceName,
                         PresenceRule::Action action,
                         QObject *parent) :
    QObject(parent),
    _id(accountId),
    _serviceName(serviceName),
    _action(action)
{
}

Accounts::AccountId PresenceRule::accountId() const
{
    return _id;
}

const QString &PresenceRule::serviceName() const
{
    return _serviceName;
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

PresenceRule &PresenceRule::operator =(const PresenceRule &other)
{
    _id = other._id;
    _serviceName = other._serviceName;
    _action = other._action;

    return *this;
}

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const PresenceRule &obj) {
    dbusArgument.beginStructure();
    dbusArgument << obj.accountId();
    dbusArgument << obj.serviceName();
    dbusArgument << int(obj.action());
    dbusArgument.endStructure();

    return dbusArgument;
}

const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, PresenceRule &obj) {
    int action;

    dbusArgument.beginStructure();
    dbusArgument >> obj._id;
    dbusArgument >> obj._serviceName;
    dbusArgument >> action;
    dbusArgument.endStructure();

    obj._action = (PresenceRule::Action) action;

    return dbusArgument;
}
