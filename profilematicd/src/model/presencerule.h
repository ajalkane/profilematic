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

#ifndef ACCOUNTRULE_H
#define ACCOUNTRULE_H

#include <QObject>
#include <QMetaType>
#include <QDBusArgument>
#include <qplatformdefs.h>
#ifdef MEEGO_EDITION_HARMATTAN
#include <Accounts/Account>
#else
namespace Accounts {
    typedef quint32 AccountId;
}
#endif

// Required for namespaced AccountId
Q_DECLARE_METATYPE(Accounts::AccountId);

class PresenceRule : public QObject
{
    Q_OBJECT
    Q_ENUMS(Action)
    Q_PROPERTY(Accounts::AccountId accountId READ accountId)
    Q_PROPERTY(QString serviceName READ serviceName)
    Q_PROPERTY(enum Action action READ action WRITE setAction NOTIFY actionChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage WRITE setStatusMessage NOTIFY statusMessageChanged)
public:
    enum Action {
        SetOnline,
        SetOffline,
        Retain
    };
public:
    explicit PresenceRule(const PresenceRule &presenceRule, QObject *parent = 0);
    explicit PresenceRule(QObject *parent = 0);
    explicit PresenceRule(const Accounts::AccountId &accountId,
                          const QString &serviceName,
                          Action action,
                          const QString &statusMessage = QString(),
                          QObject *parent = 0);

    Accounts::AccountId accountId() const;

    const QString &serviceName() const;

    Action action() const;
    void setAction(const Action action);

    const QString &statusMessage() const;
    void setStatusMessage(const QString &statusMessage);

    PresenceRule &operator=(const PresenceRule &other);
signals:
    void actionChanged();
    void statusMessageChanged();
private:
    friend const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, PresenceRule &obj);

    Accounts::AccountId _id;
    QString _serviceName;
    Action _action;
    QString _statusMessage;
};

Q_DECLARE_METATYPE(PresenceRule *)
Q_DECLARE_METATYPE(PresenceRule)

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const PresenceRule &obj);
const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, PresenceRule &obj);

#endif // ACCOUNTRULE_H
