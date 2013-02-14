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

#include "qmlruleutil.h"

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

    recalculateVisible();
}

void
QmlBaseRuleEditModel::recalculateVisible() {
    qDebug("QmlBaseRuleEditModel::recalculateVisible");
    bool hasVisibleChanged = false;
    foreach (Description *d, _descriptions) {
        bool newVisible = d->initiallyVisible || d->isSet(*_editRule);
        if (newVisible != d->visible) {
            d->visible = newVisible;
            hasVisibleChanged = true;
        }
    }

    QModelIndex modelIndexStart = createIndex(0, 0);
    QModelIndex modelIndexEnd   = createIndex(_descriptions.size() - 1, 0);
    emit dataChanged(modelIndexStart, modelIndexEnd);
    if (hasVisibleChanged) {
        qDebug("QmlBaseRuleEditModel::visibleChanged");
        emit visibleChanged();
    }
}

void
QmlBaseRuleEditModel::initializeEdit() {
    qDebug("QmlBaseRuleEditModel::initializeEdit");
    bool hasVisibles = false;
    bool hasVisibleChanged = false;
    foreach (Description *d, _descriptions) {
        bool newVisible = d->isSet(*_editRule);
        if (newVisible != d->visible) {
            d->visible = newVisible;
            hasVisibleChanged = true;
        }
        if (newVisible) hasVisibles = true;
        d->initiallyVisible = newVisible;
    }

    if (!hasVisibles) {
        foreach (Description *d, _descriptions) {
                bool newVisible = d->defaultVisible;
                if (newVisible != d->visible) {
                    d->visible = newVisible;
                    hasVisibleChanged = true;
                }
                d->visible = d->defaultVisible;
                d->initiallyVisible = d->visible;
        }
    }

    QModelIndex modelIndexStart = createIndex(0, 0);
    QModelIndex modelIndexEnd   = createIndex(_descriptions.size() - 1, 0);
    emit dataChanged(modelIndexStart, modelIndexEnd);
    if (hasVisibleChanged) emit visibleChanged();
}

void
QmlBaseRuleEditModel::clearEditItem(int index) {
    qDebug("QmlBaseRuleEditModel::clearEditItem(%d)", index);
    if (index < 0 || index >= _descriptions.count()) {
        qWarning("QmlBaseRuleEditModel::clearEditItem: Invalid index %d/%d", index, _descriptions.size());
        return;
    }

    Description *d = _descriptions.at(index);
    qDebug("QmlBaseRuleEditModel::clearEditItem(%d) %s", index, qPrintable(d->topic));
    d->clear(*_editRule);
}
