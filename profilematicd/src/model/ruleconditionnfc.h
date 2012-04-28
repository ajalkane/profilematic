/**********************************************************************
 * Copyright 2011-2012 Arto Jalkanen
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
#ifndef RULECONDITIONNFC_H
#define RULECONDITIONNFC_H

#include <QObject>
#include <QMetaType>
#include <QDBusArgument>
#include <QSet>
#include <QByteArray>

class RuleConditionNFC : public QObject
{
    Q_OBJECT

    QSet<QByteArray> _uids;
    bool _toggleCondition;

    Q_PROPERTY(QVariantList uids READ getUidsQml WRITE setUidsQml NOTIFY uidsChanged)
    Q_PROPERTY(bool toggleCondition READ getToggleCondition WRITE setToggleCondition NOTIFY toggleConditionChanged)

    void _init();
signals:
    void changed();

    void uidsChanged();
    void toggleConditionChanged();

public:
    explicit RuleConditionNFC(const RuleConditionNFC &conditionNFC, QObject *parent = 0);
    explicit RuleConditionNFC(QObject *parent = 0);

    RuleConditionNFC &operator=(const RuleConditionNFC &other);

    inline const QSet<QByteArray> &getUids() const { return _uids; }
    void setUids(const QSet<QByteArray> &uids);
    // For QML
    QVariantList getUidsQml() const;
    void setUidsQml(const QVariantList &uids);

    inline bool getToggleCondition() const { return _toggleCondition; }
    void setToggleCondition(bool toggle);

    inline bool operator==(const RuleConditionNFC &o) const {
        return this->_uids == o._uids
            && this->_toggleCondition == o._toggleCondition;
    }

};

Q_DECLARE_METATYPE(RuleConditionNFC *)
Q_DECLARE_METATYPE(RuleConditionNFC)

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const RuleConditionNFC &obj);
const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, RuleConditionNFC &obj);

#endif // RULECONDITIONNFC_H
