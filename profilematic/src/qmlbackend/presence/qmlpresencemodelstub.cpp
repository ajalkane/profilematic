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

#include "qmlpresencemodelstub.h"
#include "../../profilematicd/src/model/rule.h"
#include "../../profilematicd/src/model/presencerule.h"

#include <QDebug>

QmlPresenceModelStub::QmlPresenceModelStub(QObject *parent) :
    QmlPresenceModel(parent)
{
    appendRow(1, "SIP");
    appendRow(2, "Google Talk");
    appendRow(3, "Change to remove random");
    appendRow(4, "Change to add random");

    onRuleChanged();

    connect(this, SIGNAL(ruleChanged()), SLOT(onRuleChanged()));
}

QVariant QmlPresenceModelStub::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= _rows.count())
        return QVariant();

    const StubAccountEntry *entry = static_cast<const StubAccountEntry *>(_rows[index.row()]);

    switch (role) {
    case AccountNameRole:
        return entry->name;
    case AccountIdRole:
        return entry->id;
    case AccountEnabledRole:
        return entry->enabled;
    case PresenceRuleRole:
        return QVariant::fromValue(entry->rule);
    case AccountIconRole:
        return entry->icon;
    default:
        return QVariant();
    }
}

void QmlPresenceModelStub::onRuleChanged() {
    connect(_rows[2]->rule, SIGNAL(actionChanged()), SLOT(removeRandom()));
    connect(_rows[3]->rule, SIGNAL(actionChanged()), SLOT(addRandom()));
}

void QmlPresenceModelStub::addRandom()
{
    qDebug() << "Adding random entry";
    beginInsertRows(QModelIndex(), _rows.count(), _rows.count());
    appendRow(qrand() + 10, QString::number(qrand()));
    endInsertRows();
}

void QmlPresenceModelStub::removeRandom() {
    int row = qrand() % _rows.count();

    while (_rows[row]->rule->accountId() == 3 || _rows[row]->rule->accountId() == 4)
        row = qrand() % _rows.count();

    qDebug() << "Removing entry at row" << row;

    beginRemoveRows(QModelIndex(), row, row);
    delete _rows[row];
    _rows.removeAt(row);
    endRemoveRows();
}

void QmlPresenceModelStub::appendRow(int id,
                                     QString name,
                                     QString icon,
                                     bool enabled)
{
    _rows.append(new StubAccountEntry(id, name, createPresenceRule(id, "test"), icon, enabled));
}

QmlPresenceModelStub::StubAccountEntry::StubAccountEntry(int id,
                                                         QString name,
                                                         PresenceRule *presenceRule,
                                                         QString icon,
                                                         bool enabled) :
    AccountEntry(presenceRule),
    id(id),
    name(name),
    icon(icon),
    enabled(enabled)
{
}
