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
#ifndef QMLRULESMODEL_H
#define QMLRULESMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QVariantList>
#include <QVariantMap>

#include "../profilematicclient.h"

// TODO need common sources or library for this
#include "../../profilematicd/src/model/rule.h"

class QmlRulesModel: public QAbstractListModel
{
    Q_OBJECT

    typedef QAbstractListModel super;

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

    QList<Rule> _rules;
    Rule _editRule;

    ProfileMaticClient *_client;

    QHash<int, QByteArray> _roleToProperty;

    int _findRole(const QHash<int, QByteArray> &from, const QString &property);

    void _setProperty(int index, const QString &key, const QVariant &value);

    int _findRuleIndexById(const Rule::IdType &id) const;
public:
    enum RuleRoles {
        RuleIdRole = Qt::UserRole + 1,
        RuleActiveRole,
        RuleNameRole,
        TimeStartRole,
        // This is a dummy role and can not be accessed/set using data/setData.
        // Instead getDayIndices/setDayIndices must be used.
        DaysRole,
        ProfileRole,
        ProfileVolumeRole,
        DaysSummaryRole,

        RuleRole
    };

    QmlRulesModel(ProfileMaticClient *client, QObject *parent = 0);

    // void newRule(const QString &ruleName, const QTime &timeStart, const QString &profile);

    // QAbstractListModel implementations
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    // bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    // bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
//    Qt::ItemFlags flags ( const QModelIndex & index ) const;

    // Functions as in QML's ListModel that are needed
//    Q_INVOKABLE void move(int sourceIndex, int targetIndex, int size);
//    Q_INVOKABLE void set(int index, const QVariantMap &dict);
//    Q_INVOKABLE void append(const QVariantMap &dict);
//    Q_INVOKABLE void remove(int index);

    Q_INVOKABLE void setEditRule(int index);
    Q_INVOKABLE void setNewEditRule();
    Q_INVOKABLE Rule *getEditRule();
    Q_INVOKABLE void saveEditRule();

    // More complex data access functions that are not easily representable by roles
//    Q_INVOKABLE QVariantList getDayIndices(int index) const;
//    Q_INVOKABLE void setDayIndices(int index, const QVariantList &dayIndices);
    Q_INVOKABLE QString getDaysSummaryText(const QVariantList &dayIndices) const;

    QString getDaysSummaryText(const QSet<int> &days) const;

public slots:
    void emitSizeChanged(const QModelIndex & parent, int start, int end);

    void ruleUpdated(const Rule &);
    void ruleAppended(const Rule &);
signals:
    void sizeChanged();
};

#endif // QMLRULESMODEL_H
