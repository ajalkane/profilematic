/**********************************************************************
 * Copyright 2011 Arto Jalkanen
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
#include <QVariant>
#include <QTime>
#include <QMap>
#include <QSet>

#include "qmlrulesmodel.h"

QmlRulesModel::QmlRulesModel(ProfileMaticClient *client, QObject *parent)
    : QAbstractListModel(parent), _isActive(false), _backendError(false), _client(client)
{    
    _roleToProperty[RuleIdRole]          = "ruleId";
    _roleToProperty[RuleActiveRole]      = "ruleActive";
    _roleToProperty[RuleNameRole]        = "ruleName";
    _roleToProperty[TimeStartRole]       = "timeStart";
    _roleToProperty[DaysRole]            = "daysRole";
    _roleToProperty[ProfileRole]         = "profile";
    _roleToProperty[ProfileVolumeRole]   = "profileVolume";
    _roleToProperty[DaysSummaryRole]     = "daysSummary";

    _roleToProperty[RuleRole] = "rule";

    setRoleNames(_roleToProperty);

    connect(_client, SIGNAL(ruleUpdated(const Rule &)), this, SLOT(ruleUpdated(const Rule &)));
    connect(_client, SIGNAL(ruleAppended(const Rule &)), this, SLOT(ruleAppended(const Rule &)));
    connect(_client, SIGNAL(ruleRemoved(const QString &)), this, SLOT(ruleRemoved(const QString &)));
    connect(_client, SIGNAL(ruleMoved(const QString &, int)), this, SLOT(ruleMoved(const QString &, int)));
    connect(_client, SIGNAL(activeChanged(bool)), this, SLOT(activeChangedBackend(bool)));

    _rules = _client->getRules();
    // This should be fairly uncommon occurence, meaning profilematic daemon
    // is not running. At least for now only check it on first call.
    if (_client->lastError() != 0) {
        qDebug("QmlRulesModel::QmlRulesModel: getRules resulted in DBus error type %d", _client->lastError());
        _backendError = true;
    }
    _isActive = _client->isActive();

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    // TODO testing, away
    //    newRule("Rule 1", QTime(10, 20), "Beep");
    //    newRule("Rule 2", QTime(11, 21), "Ringing");
    //    newRule("Rule 3", QTime(12, 22), "Silent");
}

//Qt::ItemFlags
//QmlRulesModel::flags(const QModelIndex &index) const {
//    return super::flags(index) | Qt::ItemIsEditable;
//}

int
QmlRulesModel::rowCount(const QModelIndex &) const {
    return _rules.count();
}

QVariant
QmlRulesModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= _rules.count())
        return QVariant();

    const Rule &rule = _rules.at(index.row());
    switch (role) {
    case RuleRole:
        qDebug("Returning RuleRole for %s", qPrintable(rule.getRuleName()));
        return QVariant::fromValue(rule);
    case RuleIdRole:
        return rule.getRuleId();
    case RuleActiveRole:
        // return rule.ruleActive;
    case RuleNameRole:
        return rule.getRuleName();        
    case TimeStartRole:
        return rule.getTimeStartQml();
    // return rule.getTimeStart();
    case DaysRole:
        qDebug("QmlRulesModel::data role 'days' should not be directly accessed");
        break;
    case ProfileRole:
        return rule.getProfile();
    case ProfileVolumeRole:
        return rule.getProfileVolume();
    case DaysSummaryRole:
        return getDaysSummaryText(rule.getDays());
    default:
        qDebug("Unrecognized role for QmlRulesModel::data: %d", role);
        return QVariant();
    }

    return QVariant();
}

//bool
//QmlRulesModel::setData(const QModelIndex &index, const QVariant &value, int role) {
//    if (index.row() < 0 || index.row() >= _rules.count()) {
//        qDebug("QmlRulesModel::setData: Invalid index %d", index.row());
//        return false;
//    }
//    Rule &rule = _rules[index.row()];
//    switch (role) {
//    case RuleIdRole:
//        rule.setRuleId(value.toString());
//        break;
//    case RuleActiveRole:
//        // ruleActive = value.toBool();
//        break;
//    case RuleNameRole:
//        break;
//    {
//        // TODO this to daemon code
//        QString ruleName = value.toString();
//        if (ruleName.trimmed().isEmpty()) {
//            // Could be smarter rule, for example basing it on the rest of the data. But then
//            // this logic would have to be a bit higher in the hierarchy. On the other hand,
//            // if automatic rule name is wanted to be supported, it should be dynamic (in qml)
//            // This will do for now.
//            ruleName = QString("Rule #") + QString::number(index.row() + 1);
//        }
//        // rule.setRuleName(ruleName);
//        Rule updateRule = rule;
//        updateRule.setRuleName(ruleName);
//        qDebug("QmlRulesModel: client->updateRule %s", qPrintable(updateRule.getRuleName()));
//        _client->updateRule(updateRule);
//    }
//    break;
//    case TimeStartRole:
//        rule.setTimeStart(QTime::fromString(value.toString(), "hh:mm"));
//        qDebug("Set startTime to %d:%d", rule.getTimeStart().hour(), rule.getTimeStart().minute());
//        break;
//    case DaysRole:
//        // Skip silently. Only used with dummymodels.
//        return false;
//    case ProfileRole:
//        rule.setProfile(value.toString());
//        break;
//    case ProfileVolumeRole:
//        rule.setProfileVolume(value.toInt());
//        break;
//    default:
//        qDebug("Unrecognized role for QmlRulesModel::setData: %d", role);
//        return false;
//    }

//    emit dataChanged(index, index);
//    return true;
//}

//void
//QmlRulesModel::move(int sourceIndex, int targetIndex, int size) {
//    // This looks perverse, just see the documentation for QAbstractItemModel.beginMoveColumns to make sense of it
//    int targetIndexFinal = (targetIndex < sourceIndex ? targetIndex + size - 1 : targetIndex + size);
//    if (size == 1 && beginMoveRows(QModelIndex(), sourceIndex, sourceIndex + size - 1, QModelIndex(), targetIndexFinal)) {
//        qDebug("Doing moving %d -> %d size %d", sourceIndex, targetIndex, size);
//        _rules.move(sourceIndex, targetIndex);
//        endMoveRows();
//    } else {
//        qDebug("Invalid move %d -> %d size %d", sourceIndex, targetIndex, size);
//    }
//}

//int
//QmlRulesModel::_findRole(const QHash<int, QByteArray> &from, const QString &property) {
//    QHash<int, QByteArray>::const_iterator i;
//    for (i = from.constBegin(); i != from.constEnd(); ++i) {
//        if (i.value() == property) {
//            return i.key();
//        }
//    }
//    return -1;
//}

//void
//QmlRulesModel::_setProperty(int index, const QString &key, const QVariant &value) {
//    int role = _findRole(_roleToProperty, key);

//    if (role >= 0) {
//        QModelIndex modelIndex = createIndex(index, 0);
//        setData(modelIndex, value, role);
//    } else {
//        qDebug("Role not found for QML property %s, index %d", key.toLatin1().constData(), index);
//        return;
//    }
//}

//void
//QmlRulesModel::set(int index, const QVariantMap &dict) {
//    if (index < 0 || index >= _rules.count()) {
//        qDebug("QmlRulesModel::set invalid index %d, size %d", index, _rules.size());
//        return;
//    }

//    // IMPROVE: this could probably be made much more efficiently
//    // by having setData_internal function that doesn't emit
//    // dataChanged, and just calling emit dataChanged after for loop
//    QVariantMap::const_iterator i = dict.constBegin();
//    for (; i != dict.constEnd(); ++i) {
//        _setProperty(index, i.key(), i.value());
//    }
//}

void
QmlRulesModel::ruleUpdated(const Rule &rule) {
    qDebug("QmlRulesModel: Received rule updated %s %s", qPrintable(rule.getRuleId()), qPrintable(rule.getRuleName()));
    int targetIndex = _findRuleIndexById(rule.getRuleId());
    if (targetIndex < 0) {
        qDebug("QmlRulesModel: no rule found for id %s", qPrintable(rule.getRuleId()));
        return;
    }

    _rules[targetIndex] = rule;

    qDebug("QmlRulesModel: Received rule updated, now (%d) %s %s",targetIndex, qPrintable(_rules[targetIndex].getRuleId()), qPrintable(rule.getRuleName()));

    QModelIndex modelIndex = createIndex(targetIndex, 0);

    emit dataChanged(modelIndex, modelIndex);
}

void
QmlRulesModel::ruleAppended(const Rule &rule) {
    qDebug("QmlRulesModel: Received rule appended %s %s", qPrintable(rule.getRuleId()), qPrintable(rule.getRuleName()));

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _rules << rule;
    endInsertRows();
}

void
QmlRulesModel::ruleRemoved(const QString &ruleId) {
    qDebug("QmlRulesModel: Received rule removed %s", qPrintable(ruleId));
    int targetIndex = _findRuleIndexById(ruleId);
    if (targetIndex < 0) {
        qDebug("QmlRulesModel::ruleRemoved no rule found for id %s", qPrintable(ruleId));
        return;
    }

    beginRemoveRows(QModelIndex(), targetIndex, targetIndex);
    _rules.removeAt(targetIndex);
    endRemoveRows();;
}

void
QmlRulesModel::ruleMoved(const QString &ruleId, int toIndex) {
    qDebug("QmlRulesModel: Received rule removed %s", qPrintable(ruleId));
    int index = _findRuleIndexById(ruleId);
    if (index < 0) {
        qDebug("QmlRulesModel::ruleMoved no rule found for id %s", qPrintable(ruleId));
        return;
    }
    if (toIndex < 0 || toIndex > _rules.size() - 1) {
        qDebug("QmlRulesModel::ruleMoved: invalid toIndex: %d, allowed range (0 - %d)", toIndex, _rules.size() - 1);
        return;
    }

    // This looks perverse, just see the documentation for QAbstractItemModel.beginMoveColumns to make sense of it
    int toIndexFinal = (toIndex < index ? toIndex : toIndex + 1);
    if (beginMoveRows(QModelIndex(), index, index, QModelIndex(), toIndexFinal)) {
        qDebug("Doing moving %d -> %d", index, toIndex);
        _rules.move(index, toIndex);
        endMoveRows();
    } else {
        qDebug("Invalid move %d -> %d", index, toIndex);
    }
}

int
QmlRulesModel::_findRuleIndexById(const Rule::IdType &id) const {
    for (int i = 0; i < _rules.size(); ++i) {
        const Rule &r = _rules.at(i);
        if (id == r.getRuleId()) {
            return i;
        }
    }
    return -1;
}

//void
//QmlRulesModel::append(const QVariantMap &dict) {
//    // IMPROVE: this could probably be made much more efficiently
//    // by having setData_internal function that doesn't emit
//    // dataChanged, and just wrapping it all between begin/endInsertRows.
//    beginInsertRows(QModelIndex(), rowCount(), rowCount());
//    _rules << Rule();
//    endInsertRows();
//    int index = _rules.size() - 1;
//    set(index, dict);
//}

//void
//QmlRulesModel::remove(int index) {
//    if (index < 0 || index >= _rules.count()) {
//        qDebug("QmlRulesModel::remove: Invalid index %d", index);
//        return;
//    }
//    beginRemoveRows(QModelIndex(), index, index);
//    _rules.removeAt(index);
//    endRemoveRows();
//}

//QVariantList
//QmlRulesModel::getDayIndices(int index) const {
//    if (index < 0 || index >= _rules.count()) {
//        qDebug("QmlRulesModel::getDayIndices: Invalid index %d", index);
//        return QVariantList();
//    }
//    const Rule &rule = _rules.at(index);

//    QVariantList dayIndices;
//    const QSet<int> &days = rule.getDays();
//    QSet<int>::const_iterator i;
//    for (i = days.constBegin(); i != days.constEnd(); ++i) {
//        int day = *i;
//        dayIndices << QVariant::fromValue(day);
//    }
//    return dayIndices;
//}

//void
//QmlRulesModel::setDayIndices(int index, const QVariantList &dayIndices) {
//    if (index < 0 || index >= _rules.count()) {
//        qDebug("QmlRulesModel::setDayIndices: Invalid index %d", index);
//        return;
//    }
//    QSet<int> days;
//    QVariantList::const_iterator i;
//    for (i = dayIndices.constBegin(); i != dayIndices.constEnd(); ++i) {
//        QVariant variant = *i;
//        int day = variant.toInt();
//        days << day;
//    }

//    Rule &rule = _rules[index];
//    rule.setDays(days);

//    QModelIndex modelIndex = createIndex(index, 0);

//    emit dataChanged(modelIndex, modelIndex);
//}

void
QmlRulesModel::emitSizeChanged(const QModelIndex &, int, int) {
    emit sizeChanged();
}

QString
QmlRulesModel::getDaysSummaryText(const QVariantList &dayIndices) const {
    QSet<int> daysSet;
    QVariantList::const_iterator i = dayIndices.constBegin();
    for (; i != dayIndices.constEnd(); ++i) {
        QVariant variant = *i;
        daysSet << variant.toInt();
    }
    return getDaysSummaryText(daysSet);
}

QString
QmlRulesModel::getDaysSummaryText(const QSet<int> &days) const {
    qDebug("QmlRulesModel::getDaysSummaryText(), days size %d", days.size());

    if (days.size() == 7) {
        return "All days";
    }
    if (days.size() == 0) {
        return "No days";
    }

    int rangeStart = -1;

    QString daysStr = "";
    // The loop goes up-to 7 so that Sunday is considered as last in range.
    // Days.contains for 7 should alway contain false as days are in range of 0-6.
    for (int i = 0; i < 8; ++i) {
        if (days.contains(i)) {
            if (rangeStart < 0) {
                rangeStart = i;

            }
        } else {
            if (rangeStart > -1) {
                if (!daysStr.isEmpty()) {
                    daysStr += ", ";
                }

                daysStr += QDate::shortDayName(rangeStart + 1, QDate::StandaloneFormat);
                if (rangeStart < i - 1) {
                    daysStr += " - ";
                    daysStr += QDate::shortDayName((i - 1) + 1, QDate::StandaloneFormat);
                }
                rangeStart = -1;
            }
        }
    }
    return daysStr;
}


//Rule *
//QmlRulesModel::getEditRule(int index) {
//    qDebug("QmlRulesModel::getEditRule(%d)", index);
//    if (index < 0 || index >= _rules.count()) {
//        qDebug("QmlRulesModel::getRule: Invalid index %d", index);
//        return NULL; // QVariant(); // Rule();
//    }

//    return &(_rules[index]); // QVariant::fromValue(_rules.at(index));
//    // return QVariant::fromValue(_rules.at(index));
//}

void
QmlRulesModel::setEditRule(int index) {
    qDebug("QmlRulesModel::setEditRule(%d)", index);
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::setRule: Invalid index %d", index);
        _editRule = Rule();
        return;
    }

    _editRule = _rules[index];

    qDebug("QmlRulesModel::setEditRule client->saveEditRule id %s, name %s", qPrintable(_editRule.getRuleId()), qPrintable(_editRule.getRuleName()));
}

void
QmlRulesModel::setNewEditRule() {
    qDebug("QmlRulesModel::setNewEditRule");

    _editRule = Rule();
    // When creating new rule, automatically select all days
    QSet<int> days;
    for (int i = 0; i < 7; i++) {
        days << i;
    }
    _editRule.setDays(days);
}

Rule *
QmlRulesModel::getEditRule() {
    return &_editRule;
}

void
QmlRulesModel::saveEditRule() {
    // TODO away debug, at least check indices if nothing else
    qDebug("QmlRulesModel::saveEditRule client->saveEditRule id %s, name %s", qPrintable(_editRule.getRuleId()), qPrintable(_editRule.getRuleName()));
    if (_editRule.getRuleId().isEmpty()) {
        _client->appendRule(_editRule);
    } else {
        _client->updateRule(_editRule);
    }
}

void
QmlRulesModel::removeRule(int index) {
    qDebug("QmlRulesModel::removeRule(%d)", index);
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::removeRule: Invalid index %d", index);
        return;
    }

    const Rule &r = _rules.at(index);
    _client->removeRule(r.getRuleId());
}

void
QmlRulesModel::moveRule(int index, int toIndex) {
    qDebug("QmlRulesModel::moveRule(%d, %d)", index, toIndex);
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::moveRule: Invalid index %d", index);
        return;
    }

    const Rule &r = _rules.at(index);
    _client->moveRule(r.getRuleId(), toIndex);
}

bool
QmlRulesModel::isActive() const {
    return _isActive;
}

void
QmlRulesModel::setActive(bool isActive) {
    _client->setActive(isActive);
}

void
QmlRulesModel::activeChangedBackend(bool isActive) {
    if (isActive != _isActive) {
        _isActive = isActive;
        emit activeChanged();
    }
}

bool
QmlRulesModel::backendError() const {
    return _backendError;
}
