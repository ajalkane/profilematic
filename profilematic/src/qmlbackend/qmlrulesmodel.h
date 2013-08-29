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

#include "model/rule.h"

#include "qmlconditioneditmodel.h"
#include "qmlactioneditmodel.h"

#include "qmlprofilesmodel.h"
#include "../profilematicclient.h"


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

    ProfileMaticClient    *_client;
    QmlProfilesModel      *_profilesModel;
    QmlConditionEditModel *_conditionEditModel;
    QmlActionEditModel    *_actionEditModel;

    bool _isMissingDeviceModeCredential;
    bool _noFormatToLower;

    QHash<int, QByteArray> _roleToProperty;
    QSet<Rule::IdType> _matchingRuleIds;
    mutable QHash<Rule::IdType, QString> _ruleSummaryCache;

    void _initializeNoFormatToLower();

    int _findRole(const QHash<int, QByteArray> &from, const QString &property);

    void _setProperty(int index, const QString &key, const QVariant &value);

    int _findRuleIndexById(const Rule::IdType &id) const;

    QString _ruleSummaryText(const Rule &rule) const;
    QString _createRuleSummaryText(const Rule *rule, const QString &nonUsableRuleString) const;
public:
    // IMPROVE: many of these roles are really not used anymore. Clean-up.
    enum RuleRoles {
        IsDefaultRuleRole = Qt::UserRole + 1,
        RuleActiveRole,
        RuleNameRole,
        RuleSummaryRole,
        MatchingRole,
        StopIfMatchedRole
    };

    QmlRulesModel(ProfileMaticClient *client, QmlProfilesModel *profilesModel, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void setEditRule(int index);
    Q_INVOKABLE void setNewEditRule();
    Q_INVOKABLE void setNewEditRuleFrom(int index);
    Q_INVOKABLE Rule *getEditRule();
    Q_INVOKABLE void saveEditRule();
    Q_INVOKABLE void removeRule(int index);
    Q_INVOKABLE void toggleRuleActive(int index);
    Q_INVOKABLE void moveRule(int fromIndex, int toIndex);

    QString getDaysSummaryText(const QSet<int> &days) const;
    // For QML
    Q_INVOKABLE QString getDaysSummaryText(const QVariantList &dayIndices) const;
    Q_INVOKABLE QString getTimeSummaryText(const RuleCondition *condition, const QString &nonUsableTimeString) const;
    Q_INVOKABLE QString getTimeSummaryText(RuleCondition *condition, const QString &nonUsableTimeString) const;
    Q_INVOKABLE QString getRuleSummaryText(Rule *rule, const QString &nonUsableTimeString) const;
    Q_INVOKABLE void testCommandLine(const QString &commandLine) const;
    Q_INVOKABLE void executeAction(int index) const;
    Q_INVOKABLE inline bool isMissingDeviceModeCredential() const { return _isMissingDeviceModeCredential; }

    bool isActive() const;
    void setActive(bool isActive);
    bool backendError() const;
    inline QmlConditionEditModel *getConditionEditModel() const { return _conditionEditModel; }
    inline QmlActionEditModel    *getActionEditModel() const { return _actionEditModel; }
public slots:
    void emitSizeChanged(const QModelIndex & parent, int start, int end);

    void matchingRuleIdsChanged(const QStringList &matchingRuleIds);
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
