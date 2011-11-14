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

QmlRulesModel::QmlRulesModel(const QList<Rule> &rules, QObject *parent)
    : QAbstractListModel(parent), _rules(rules)
{
    _roleToProperty[RuleIdRole]          = "ruleId";
    _roleToProperty[RuleActiveRole]      = "ruleActive";
    _roleToProperty[RuleNameRole]        = "ruleName";
    _roleToProperty[TimeStartRole]       = "timeStart";
    _roleToProperty[DaysRole]            = "daysRole";
    _roleToProperty[ProfileRole]         = "profile";
    _roleToProperty[ProfileVolumeRole]   = "profileVolume";
    _roleToProperty[DaysSummaryRole]     = "daysSummary";
    setRoleNames(_roleToProperty);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    // TODO testing, away
    //    newRule("Rule 1", QTime(10, 20), "Beep");
    //    newRule("Rule 2", QTime(11, 21), "Ringing");
    //    newRule("Rule 3", QTime(12, 22), "Silent");
}

Qt::ItemFlags
QmlRulesModel::flags(const QModelIndex &index) const {
    return super::flags(index) | Qt::ItemIsEditable;
}

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
    case RuleIdRole:
        return rule.getRuleId();
    case RuleActiveRole:
        // return rule.ruleActive;
    case RuleNameRole:
        return rule.getRuleName();
    case TimeStartRole:
    {
        QTime time = rule.getTimeStart();
        int hour = time.hour();
        int min  = time.minute();
        QString timeStr;
        if (hour < 10) {
            timeStr += "0";
        }
        timeStr += QString::number(hour);
        timeStr += ":";
        if (min < 10) {
            timeStr += "0";
        }
        timeStr += QString::number(min);
        qDebug("QmlRulesModel::timeStartRole returning '%s'", qPrintable(timeStr));
        return timeStr;
    }
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
        qDebug("Unrecognized role for QmlRulesModel::setData: %d", role);
        return QVariant();
    }

    return QVariant();
}

bool
QmlRulesModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.row() < 0 || index.row() >= _rules.count()) {
        qDebug("QmlRulesModel::setData: Invalid index %d", index.row());
        return false;
    }
    Rule &rule = _rules[index.row()];
    switch (role) {
    case RuleIdRole:
        rule.setRuleId(value.toInt());
        break;
    case RuleActiveRole:
        // ruleActive = value.toBool();
        break;
    case RuleNameRole:
    {
        QString ruleName = value.toString();
        if (ruleName.trimmed().isEmpty()) {
            // Could be smarter rule, for example basing it on the rest of the data. But then
            // this logic would have to be a bit higher in the hierarchy. On the other hand,
            // if automatic rule name is wanted to be supported, it should be dynamic (in qml)
            // This will do for now.
            ruleName = QString("Rule #") + QString::number(index.row() + 1);
        }
        rule.setRuleName(ruleName);
    }
    break;
    case TimeStartRole:
        rule.setTimeStart(QTime::fromString(value.toString(), "hh:mm"));
        qDebug("Set startTime to %d:%d", rule.getTimeStart().hour(), rule.getTimeStart().minute());
        break;
    case DaysRole:
        // Skip silently. Only used with dummymodels.
        return false;
    case ProfileRole:
        rule.setProfile(value.toString());
        break;
    case ProfileVolumeRole:
        rule.setProfileVolume(value.toInt());
        break;
    default:
        qDebug("Unrecognized role for QmlRulesModel::setData: %d", role);
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

void
QmlRulesModel::move(int sourceIndex, int targetIndex, int size) {
    // This looks perverse, just see the documentation for QAbstractItemModel.beginMoveColumns to make sense of it
    int targetIndexFinal = (targetIndex < sourceIndex ? targetIndex + size - 1 : targetIndex + size);
    if (size == 1 && beginMoveRows(QModelIndex(), sourceIndex, sourceIndex + size - 1, QModelIndex(), targetIndexFinal)) {
        qDebug("Doing moving %d -> %d size %d", sourceIndex, targetIndex, size);
        _rules.move(sourceIndex, targetIndex);
        endMoveRows();
    } else {
        qDebug("Invalid move %d -> %d size %d", sourceIndex, targetIndex, size);
    }
}

int
QmlRulesModel::_findRole(const QHash<int, QByteArray> &from, const QString &property) {
    QHash<int, QByteArray>::const_iterator i;
    for (i = from.constBegin(); i != from.constEnd(); ++i) {
        if (i.value() == property) {
            return i.key();
        }
    }
    return -1;
}

void
QmlRulesModel::_setProperty(int index, const QString &key, const QVariant &value) {
    int role = _findRole(_roleToProperty, key);

    if (role >= 0) {
        QModelIndex modelIndex = createIndex(index, 0);
        setData(modelIndex, value, role);
    } else {
        qDebug("Role not found for QML property %s, index %d", key.toLatin1().constData(), index);
        return;
    }
}

void
QmlRulesModel::set(int index, const QVariantMap &dict) {
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::set invalid index %d, size %d", index, _rules.size());
        return;
    }

    // IMPROVE: this could probably be made much more efficiently
    // by having setData_internal function that doesn't emit
    // dataChanged, and just calling emit dataChanged after for loop
    QVariantMap::const_iterator i = dict.constBegin();
    for (; i != dict.constEnd(); ++i) {
        _setProperty(index, i.key(), i.value());
    }
}

void
QmlRulesModel::append(const QVariantMap &dict) {
    // IMPROVE: this could probably be made much more efficiently
    // by having setData_internal function that doesn't emit
    // dataChanged, and just wrapping it all between begin/endInsertRows.
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _rules << Rule();
    endInsertRows();
    int index = _rules.size() - 1;
    set(index, dict);    
}

void
QmlRulesModel::remove(int index) {
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::remove: Invalid index %d", index);
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    _rules.removeAt(index);
    endRemoveRows();
}

QVariantList
QmlRulesModel::getDayIndices(int index) const {
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::getDayIndices: Invalid index %d", index);
        return QVariantList();
    }
    const Rule &rule = _rules.at(index);

    QVariantList dayIndices;
    const QSet<int> &days = rule.getDays();
    QSet<int>::const_iterator i;
    for (i = days.constBegin(); i != days.constEnd(); ++i) {
        int day = *i;
        dayIndices << QVariant::fromValue(day);
    }
    return dayIndices;
}

void
QmlRulesModel::setDayIndices(int index, const QVariantList &dayIndices) {
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::setDayIndices: Invalid index %d", index);
        return;
    }
    QSet<int> days;
    QVariantList::const_iterator i;
    for (i = dayIndices.constBegin(); i != dayIndices.constEnd(); ++i) {
        QVariant variant = *i;
        int day = variant.toInt();
        days << day;
    }

    Rule &rule = _rules[index];
    rule.setDays(days);

    QModelIndex modelIndex = createIndex(index, 0);

    emit dataChanged(modelIndex, modelIndex);
}

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
