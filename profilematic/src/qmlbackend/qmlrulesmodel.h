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

// IMPROVE need to have the Rule object in common sources or library.
#include "../../profilematicd/src/model/rule.h"

class QmlRulesModel: public QAbstractListModel
{
    Q_OBJECT

    typedef QAbstractListModel super;

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool backendError READ backendError NOTIFY backendErrorChanged)

    QList<Rule> _rules;
    Rule _editRule;
    bool _isActive;
    bool _backendError;

    ProfileMaticClient *_client;

    QHash<int, QByteArray> _roleToProperty;

    int _findRole(const QHash<int, QByteArray> &from, const QString &property);

    void _setProperty(int index, const QString &key, const QVariant &value);

    int _findRuleIndexById(const Rule::IdType &id) const;
public:
    enum RuleRoles {
        RuleIdRole = Qt::UserRole + 1,
        IsDefaultRuleRole,
        RuleActiveRole,
        RuleNameRole,
        TimeStartRole,
        TimeEndRole,
        // This is a dummy role and can not be accessed/set using data/setData.
        // Instead getDayIndices/setDayIndices must be used.
        DaysRole,
        ProfileRole,
        ProfileVolumeRole,
        DaysSummaryRole,
        FlightModeRole
    };

    QmlRulesModel(ProfileMaticClient *client, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void setEditRule(int index);
    Q_INVOKABLE void setNewEditRule();
    Q_INVOKABLE Rule *getEditRule();
    Q_INVOKABLE void saveEditRule();
    Q_INVOKABLE void removeRule(int index);
    Q_INVOKABLE void moveRule(int fromIndex, int toIndex);

    QString getDaysSummaryText(const QSet<int> &days) const;
    // For QML
    Q_INVOKABLE QString getDaysSummaryText(const QVariantList &dayIndices) const;
    Q_INVOKABLE QString getTimeSummaryText(Rule *rule, const QString &nonUsableTimeString) const;

    bool isActive() const;
    void setActive(bool isActive);
    bool backendError() const;
public slots:
    void emitSizeChanged(const QModelIndex & parent, int start, int end);

    void ruleUpdated(const Rule &);
    void ruleAppended(const Rule &);
    void ruleRemoved(const QString &);
    void ruleMoved(const QString &, int);
    void activeChangedBackend(bool isActive);
signals:
    void sizeChanged();
    void activeChanged();
    void backendErrorChanged();
};

#endif // QMLRULESMODEL_H
