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
#include "qmlruleutil.h"

QmlRulesModel::QmlRulesModel(ProfileMaticClient *client, QmlProfilesModel *profilesModel, QObject *parent)
    : QAbstractListModel(parent), _isActive(false), _backendError(false), _client(client), _profilesModel(profilesModel),
      _isMissingDeviceModeCredential(false)
{    
    _roleToProperty[IsDefaultRuleRole]   = "isDefaultRule";
    _roleToProperty[RuleActiveRole]      = "ruleActive";
    _roleToProperty[RuleNameRole]        = "ruleName";
    _roleToProperty[RuleSummaryRole]     = "ruleSummary";
    _roleToProperty[MatchingRole]        = "matching";
    _roleToProperty[StopIfMatchedRole]   = "stopIfMatched";

    setRoleNames(_roleToProperty);

    _conditionEditModel = new QmlConditionEditModel(&_editRule, this);
    _actionEditModel = new QmlActionEditModel(&_editRule, this);

    connect(_client, SIGNAL(ruleUpdated(const Rule &)), this, SLOT(ruleUpdated(const Rule &)));
    connect(_client, SIGNAL(ruleAppended(const Rule &)), this, SLOT(ruleAppended(const Rule &)));
    connect(_client, SIGNAL(ruleRemoved(const QString &)), this, SLOT(ruleRemoved(const QString &)));
    connect(_client, SIGNAL(ruleMoved(const QString &, int)), this, SLOT(ruleMoved(const QString &, int)));
    connect(_client, SIGNAL(activeChanged(bool)), this, SLOT(activeChangedBackend(bool)));
    connect(_client, SIGNAL(matchingRuleIdsChanged(const QStringList &)), this, SLOT(matchingRuleIdsChanged(const QStringList &)));

    _rules = _client->getRules();
    // This should be fairly uncommon occurence, meaning profilematic daemon
    // is not running, except just after phone has been booted. In that case it
    // seems the UI becomes usable a minute or two before the background tasks
    // are started. At least for now only check it on first call.
    if (_client->lastError() != 0) {
        qDebug("QmlRulesModel::QmlRulesModel: getRules resulted in DBus error type %d", _client->lastError());
        _backendError = true;
    }
    _isActive = _client->isActive();
    _isMissingDeviceModeCredential = !_client->hasDeviceModeCredential();
    _matchingRuleIds = QSet<Rule::IdType>::fromList(_client->getMatchingRuleIds());

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
}

//Qt::ItemFlags
//QmlRulesModel::flags(const QModelIndex &index) const {
//    return super::flags(index) | Qt::ItemIsEditable;
//}

int
QmlRulesModel::rowCount(const QModelIndex &) const {
    return _rules.count();
}

void
QmlRulesModel::emitSizeChanged(const QModelIndex &, int, int) {
    emit sizeChanged();
}

QVariant
QmlRulesModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= _rules.count())
        return QVariant();

    const Rule &rule = _rules.at(index.row());
    switch (role) {
    case IsDefaultRuleRole:
        return rule.isDefaultRule();
    case RuleActiveRole:
        return rule.getRuleActive();
    case RuleNameRole:
        return rule.getRuleName();        
    case RuleSummaryRole:
        return _ruleSummaryText(rule);
    case MatchingRole:
        return _matchingRuleIds.contains(rule.getRuleId());
    case StopIfMatchedRole:
        return rule.getStopIfMatched();
    default:
        qDebug("Unrecognized role for QmlRulesModel::data: %d", role);
        return QVariant();
    }

    return QVariant();
}

void
QmlRulesModel::ruleUpdated(const Rule &rule) {
    // qDebug("QmlRulesModel: Received rule updated %s %s", qPrintable(rule.getRuleId()), qPrintable(rule.getRuleName()));
    int targetIndex = _findRuleIndexById(rule.getRuleId());
    if (targetIndex < 0) {
        qDebug("QmlRulesModel: no rule found for id %s", qPrintable(rule.getRuleId()));
        return;
    }

    _rules[targetIndex] = rule;

    // qDebug("QmlRulesModel: Received rule updated, now (%d) %s %s",targetIndex, qPrintable(_rules[targetIndex].getRuleId()), qPrintable(rule.getRuleName()));

    QModelIndex modelIndex = createIndex(targetIndex, 0);

    _ruleSummaryCache.remove(rule.getRuleId());

    emit dataChanged(modelIndex, modelIndex);
}

void
QmlRulesModel::matchingRuleIdsChanged(const QStringList &ruleIds) {
    qDebug() << "QmlRulesModel::matchingRuleIdsChanged" << ruleIds;
    QSet<Rule::IdType> ruleIdsSet(QSet<Rule::IdType>::fromList(ruleIds));

    QSet<Rule::IdType> notifyRuleIds(ruleIdsSet);
    notifyRuleIds.unite(_matchingRuleIds);

    _matchingRuleIds = ruleIdsSet;

    for (QSet<Rule::IdType>::const_iterator i = notifyRuleIds.constBegin(); i != notifyRuleIds.constEnd(); ++i) {
        const Rule::IdType &ruleId = *i;
        int targetIndex = _findRuleIndexById(ruleId);
        if (targetIndex < 0) {
            qDebug("QmlRulesModel:matchingRuleIdsChanged no rule found for id %s", qPrintable(ruleId));
            continue;
        }
        QModelIndex modelIndex = createIndex(targetIndex, 0);

        emit dataChanged(modelIndex, modelIndex);
    }

}

void
QmlRulesModel::ruleAppended(const Rule &rule) {
    // qDebug("QmlRulesModel: Received rule appended %s %s", qPrintable(rule.getRuleId()), qPrintable(rule.getRuleName()));

    // Assume default rule is always last, and can not be removed.
    beginInsertRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
    _rules.insert(_rules.size() - 1, rule);
    endInsertRows();
}

void
QmlRulesModel::ruleRemoved(const QString &ruleId) {
    // qDebug("QmlRulesModel: Received rule removed %s", qPrintable(ruleId));
    int targetIndex = _findRuleIndexById(ruleId);
    if (targetIndex < 0) {
        qDebug("QmlRulesModel::ruleRemoved no rule found for id %s", qPrintable(ruleId));
        return;
    }

    beginRemoveRows(QModelIndex(), targetIndex, targetIndex);
    _ruleSummaryCache.remove(ruleId);
    _rules.removeAt(targetIndex);
    endRemoveRows();;
}

void
QmlRulesModel::ruleMoved(const QString &ruleId, int toIndex) {
    // qDebug("QmlRulesModel: Received rule removed %s", qPrintable(ruleId));
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
        // qDebug("Doing moving %d -> %d", index, toIndex);
        _rules.move(index, toIndex);
        endMoveRows();
    } else {
        // qDebug("Invalid move %d -> %d", index, toIndex);
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
    return QmlRuleUtil::instance()->daysSummaryText(days);
}

QString
QmlRulesModel::getTimeSummaryText(RuleCondition *condition, const QString &nonUsableTimeString) const {
    return getTimeSummaryText(const_cast<const RuleCondition *>(condition), nonUsableTimeString);
}

QString
QmlRulesModel::getTimeSummaryText(const RuleCondition *rule, const QString &nonUsableTimeString) const {
    return QmlRuleUtil::instance()->timeSummary(rule, nonUsableTimeString);
}

QString
QmlRulesModel::getRuleSummaryText(Rule *rule, const QString &nonUsableRuleString) const {
    return _createRuleSummaryText(const_cast<const Rule *>(rule), nonUsableRuleString);
}

QString
QmlRulesModel::_ruleSummaryText(const Rule &rule) const {
    if (_ruleSummaryCache.contains(rule.getRuleId())) {
        return _ruleSummaryCache[rule.getRuleId()];
    }
    qDebug("QmlRulesModel::getRuleSummaryText not in cache for %s", qPrintable(rule.getRuleName()));
    QString summary = _createRuleSummaryText(const_cast<const Rule *>(&rule), "");
    _ruleSummaryCache.insert(rule.getRuleId(), summary);
    return summary;
}

// Simple formatting for summaries shown as list. Individual summaries
// are expected to start with upper case letter. This will put starting
// letter to lower, unless it seems to start with abbreviation (like GSM).
void formatListingSummary(QString &s) {
    if (s.length() > 1 && s.at(0).isUpper() && s.at(1).isLower()) {
        if (!s.startsWith("Bluetooth")) {
            s[0] = s.at(0).toLower();
        }
    }
}

QString
QmlRulesModel::_createRuleSummaryText(const Rule *rule, const QString &nonUsableRuleString) const {

    qDebug("QmlRulesModel::createRuleSummaryText called for %s", qPrintable(rule->getRuleName()));

    // qDebug("QmlRulesModel::getRuleSummaryText()");
    // Rule rule = ruleVariant.value<Rule>();
    if (rule == 0) {
        // qDebug("QmlRulesModel::getTimeSummaryText() null rule");
        return nonUsableRuleString;
    }

    QString action;
    QString condition;
    int numAction = 0;
    int numCondition = 0;

    QString emptyString;
    foreach (const QmlBaseRuleEditModel::Description *d, _actionEditModel->getDescriptions()) {
        QString summary = d->summary(*rule, emptyString, true);
        if (!summary.isEmpty()) {
            if (numAction > 0) {
                formatListingSummary(summary);
                action.append(tr(", ", "This is for separating actions when displaying in main view summary. Please note the space separator at end of translation string."));
            }

            action += summary;
            ++numAction;
        }
    }

    if (rule->isDefaultRule()) {
        condition += tr("other rules don't apply");
        ++numCondition;
    } else {
        foreach (const QmlBaseRuleEditModel::Description *d, _conditionEditModel->getDescriptions()) {
            QString summary = d->summary(*rule, emptyString, true);
            if (!summary.isEmpty()) {
                if (numCondition > 0) condition.append(" " + tr("and") + " ");
                formatListingSummary(summary);
                condition += summary;
                ++numCondition;
            }
        }
    }

    if ((numAction == 0 || numCondition == 0) && !rule->isDefaultRule()) {
        return nonUsableRuleString;
    }

    QString summary;
    if (numAction > 0) {
        summary.append(action);
    }
    if (numCondition > 0) {
        summary.append(numAction > 0 ? " " + tr("when") + " " : tr("When") + " ");
        summary.append(condition);
    }
    return summary;
}

void
QmlRulesModel::testCommandLine(const QString &commandLineInput) const {
    QString commandLine = commandLineInput.trimmed();
    if (!commandLine.isEmpty()) {
        _client->testCommandLine(commandLine);
    }
}

Rule *
QmlRulesModel::getEditRule() {
    return &_editRule;
}

void
QmlRulesModel::setEditRule(int index) {
    qDebug("QmlRulesModel::setEditRule(%d)", index);
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::setEditRule: Invalid index %d", index);
        _editRule = Rule();
        return;
    }

    _editRule = _rules[index];

    qDebug("QmlRulesModel::setEditRule client->saveEditRule id %s, name %s %p", qPrintable(_editRule.getRuleId()), qPrintable(_editRule.getRuleName()), &_editRule);
}

void
QmlRulesModel::setNewEditRule() {
    qDebug("QmlRulesModel::setNewEditRule");

    _editRule = Rule();
    // According to user input, this might be confusing, so do not set all days. Empty selection.
//    // When creating new rule, automatically select all days
//    QSet<int> days;
//    for (int i = 0; i < 7; i++) {
//        days << i;
//    }
//    _editRule.setDays(days);
}

void
QmlRulesModel::setNewEditRuleFrom(int index) {
    // qDebug("QmlRulesModel::setNewEditRuleFrom(%d)", index);
    if (index < 0 || index >= _rules.count()) {
        qWarning("QmlRulesModel::setNewEditRuleFrom: Invalid index %d", index);
        _editRule = Rule();
        return;
    }

    _editRule = _rules[index];
    _editRule.setRuleId("");
    _editRule.setRuleName("");

    // qDebug("QmlRulesModel::setEditRule client->saveEditRule id %s, name %s", qPrintable(_editRule.getRuleId()), qPrintable(_editRule.getRuleName()));
}

void
QmlRulesModel::saveEditRule() {
    if (_editRule.getRuleId().isEmpty()) {
        _client->appendRule(_editRule);
    } else {
        _client->updateRule(_editRule);
    }
}

void
QmlRulesModel::removeRule(int index) {
    // qDebug("QmlRulesModel::removeRule(%d)", index);
    if (index < 0 || index >= _rules.count()) {
        qWarning("QmlRulesModel::removeRule: Invalid index %d", index);
        return;
    }

    const Rule &r = _rules.at(index);
    _client->removeRule(r.getRuleId());
}

void
QmlRulesModel::toggleRuleActive(int index) {
    // qDebug("QmlRulesModel::removeRule(%d)", index);
    if (index < 0 || index >= _rules.count()) {
        qWarning("QmlRulesModel::toggleRuleActive: Invalid index %d", index);
        return;
    }

    Rule r = _rules.at(index);
    r.setRuleActive(!r.getRuleActive());
    _client->updateRule(r);
}

void
QmlRulesModel::moveRule(int index, int toIndex) {
    // qDebug("QmlRulesModel::moveRule(%d, %d)", index, toIndex);
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::moveRule: Invalid index %d", index);
        return;
    }

    const Rule &r = _rules.at(index);
    _client->moveRule(r.getRuleId(), toIndex);
}

void
QmlRulesModel::executeAction(int index) const {
    // qDebug("QmlRulesModel::executeAction(%d)", index);
    if (index < 0 || index >= _rules.count()) {
        qDebug("QmlRulesModel::executeAction: Invalid index %d", index);
        return;
    }

    const Rule &r = _rules.at(index);
    _client->executeAction(r.action());
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
