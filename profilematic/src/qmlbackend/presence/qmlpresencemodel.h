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

#ifndef QMLACCOUNTSMODEL_H
#define QMLACCOUNTSMODEL_H

#include <QAbstractListModel>
#include <QDeclarativeListProperty>

#ifdef MEEGO_EDITION_HARMATTAN
#include <Accounts/Account>
#else
namespace Accounts {
typedef quint32 AccountId;
}
#endif

class Rule;
class PresenceRule;

class QmlPresenceModel : public QAbstractListModel
{
    Q_OBJECT
    /**
      * This property holds the presence rules of this model.
      *
      * \note Each rule is owned by the model and must be copied if you require
      *       access after the model has been destructed.
      */
    Q_PROPERTY(QList<QObject *> presenceRules READ presenceRules WRITE setPresenceRules NOTIFY presenceRulesChanged)
public:
    enum Role {
        AccountNameRole = Qt::UserRole + 1,
        AccountIdRole,
        AccountEnabledRole,
        PresenceRuleRole,
        AccountIconRole
    };
public:
    explicit QmlPresenceModel(QObject *parent = 0);
    virtual ~QmlPresenceModel();

    int rowCount(const QModelIndex &parent) const;

    QList<QObject *> presenceRules() const;
    void setPresenceRules(QList<QObject *> presenceRules);
signals:
    void presenceRulesChanged();
protected:
    PresenceRule *createPresenceRule(const Accounts::AccountId &accountId,
                                     const QString &serviceName);
protected:
    struct AccountEntry {
        AccountEntry(PresenceRule *rule);

        PresenceRule *rule;
    };
protected:
    QList<AccountEntry *> _rows;
private slots:
    void onActionChanged();
};

#endif // QMLACCOUNTSMODEL_H
