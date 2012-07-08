/**********************************************************************
 * Copyright 2012 Arto Jalkanen
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

#include "qmlbaseruleeditmodel.h"

#include "qmlrulesummary.h"

QmlBaseRuleEditModel::QmlBaseRuleEditModel(Rule *editRule, QObject *parent)
    : QAbstractListModel(parent), _editRule(editRule)
{
    qDebug("QmlBaseRuleEditModel::QmlBaseRuleEditModel() editRule %p", editRule);

    _roleToProperty[TopicRole]       = "name";
    _roleToProperty[SummaryRole]     = "summary";
    _roleToProperty[QmlEditFileRole] = "qmlEditFile";
    _roleToProperty[VisibleRole]     = "visible";

    setRoleNames(_roleToProperty);
}

int
QmlBaseRuleEditModel::rowCount(const QModelIndex &) const {
    // qDebug("QmlBaseRuleEditModel::rowCount() %d", _descriptions.count());
    return _descriptions.count();
}

QVariant
QmlBaseRuleEditModel::data(const QModelIndex & index, int role) const {
    // qDebug("QmlBaseRuleEditModel::data %d/%d", index.row(), _descriptions.count());
    if (index.row() < 0 || index.row() >= _descriptions.count())
        return QVariant();

    const Description *desc = _descriptions.at(index.row());
    switch (role) {
    case TopicRole:
        return desc->topic;
    case SummaryRole:
        return desc->summary(*_editRule);
    case QmlEditFileRole:
        return desc->qmlEditFile;
    case VisibleRole:
        return desc->visible;
    default:
        qDebug("Unrecognized role for QmlBaseRuleEditModel::data: %d", role);
        return QVariant();
    }

    return QVariant();
}

void
QmlBaseRuleEditModel::ruleChanged() {
    qDebug("QmlBaseRuleEditModel::ruleChanged() _descriptions.size() = %d", _descriptions.size());
    // TODO recalculate summaries

    recalculateVisible();
}

void
QmlBaseRuleEditModel::recalculateVisible() {
    qDebug("QmlBaseRuleEditModel::recalculateVisible");
    foreach (Description *d, _descriptions) {
        d->visible = d->initiallyVisible || d->isSet(*_editRule);
    }

    QModelIndex modelIndexStart = createIndex(0, 0);
    QModelIndex modelIndexEnd   = createIndex(_descriptions.size() - 1, 0);
    emit dataChanged(modelIndexStart, modelIndexEnd);
}

void
QmlBaseRuleEditModel::initializeEdit() {
    qDebug("QmlBaseRuleEditModel::initializeEdit");
    bool hasVisibles = false;
    foreach (Description *d, _descriptions) {
        d->visible = d->isSet(*_editRule);
        d->initiallyVisible = d->visible;
        if (d->visible) hasVisibles = true;
    }

    if (!hasVisibles) {
        foreach (Description *d, _descriptions) {
                d->visible = d->defaultVisible;
                d->initiallyVisible = d->visible;
        }
    }

    QModelIndex modelIndexStart = createIndex(0, 0);
    QModelIndex modelIndexEnd   = createIndex(_descriptions.size() - 1, 0);
    emit dataChanged(modelIndexStart, modelIndexEnd);

}
