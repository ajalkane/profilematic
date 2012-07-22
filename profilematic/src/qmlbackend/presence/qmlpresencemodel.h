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

#include "../../profilematicd/src/model/ruleaction.h"

#include <QAbstractListModel>
#include <QDeclarativeListProperty>

#ifdef MEEGO_EDITION_HARMATTAN
#include <Accounts/Account>
#else
namespace Accounts {
typedef quint32 AccountId;
}
#endif

class PresenceRule;

class QmlPresenceModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(RuleAction * action READ action WRITE setAction NOTIFY actionChanged)
public:
    enum Role {
        AccountNameRole = Qt::UserRole + 1,
        AccountIdRole,
        AccountEnabledRole,
        PresenceRuleRole,
        AccountIconRole,
        SupportedPresencesRole
    };
public:
    explicit QmlPresenceModel(QObject *parent = 0);
    virtual ~QmlPresenceModel();

    int rowCount(const QModelIndex &parent) const;

    RuleAction *action() const;
    void setAction(RuleAction *action);
signals:
    void actionChanged();
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
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int start, int end);
    void onRowsInserted(const QModelIndex &parent, int start, int end);
private:
    void updatePresenceRules();
private:
    RuleAction *_action;
};

#endif // QMLACCOUNTSMODEL_H
