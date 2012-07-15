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
#ifndef QMLBASERULEEDITMODEL_H
#define QMLBASERULEEDITMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QVariantList>
#include <QVariantMap>

// IMPROVE need to have the Rule object in common sources or library.
#include "../../profilematicd/src/model/rule.h"

class QmlBaseRuleEditModel: public QAbstractListModel
{
    Q_OBJECT

public:
    class Description {
    protected:
        QString topic;
        QString qmlEditFile;
        bool visible;
        bool defaultVisible;
        bool initiallyVisible;
    public:
        inline Description(const QString &ptopic, const QString pqmlEditFile, bool pdefaultVisible = false) {
            topic = ptopic;
            qmlEditFile = pqmlEditFile;
            visible = false;
            defaultVisible = pdefaultVisible;
            initiallyVisible = false;
        }

        virtual QString summary(const Rule &, bool inListing = false) const = 0;
        virtual QString summary(const Rule &, const QString &notUsed, bool inListing = false) const = 0;
        virtual bool isSet(const Rule &) const = 0;
        virtual void clear(Rule &) const = 0;

        friend class QmlBaseRuleEditModel;
    };

private:
    typedef QAbstractListModel super;

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

    QHash<int, QByteArray> _roleToProperty;

protected:
    Rule *_editRule;
    QList<Description *> _descriptions;

    void recalculateVisible();
public:

    enum ConditionRoles {
        TopicRole = Qt::UserRole + 1,
        SummaryRole,
        QmlEditFileRole,
        VisibleRole
    };

    QmlBaseRuleEditModel(Rule *rule, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void initializeEdit();
    Q_INVOKABLE void clearEditItem(int index);

    inline const QList<Description *> &getDescriptions() const { return _descriptions; }

public slots:
    void ruleChanged();
signals:
    void sizeChanged();
    void visibleChanged();
};

#endif // QMLBASERULEEDITMODEL_H
