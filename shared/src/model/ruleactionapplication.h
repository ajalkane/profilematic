/**********************************************************************
 * Copyright 2013 Arto Jalkanen
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
#ifndef RULEACTIONAPPLICATION_H
#define RULEACTIONAPPLICATION_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QDBusArgument>

class RuleActionApplication : public QObject
{
    Q_OBJECT

    QSet<QString> _launchers;

    Q_PROPERTY(QVariantList launchers READ getLaunchersQml WRITE setLaunchersQml NOTIFY launchersChanged)

    void _init();
signals:
    void changed();

    void launchersChanged();

public:
    explicit RuleActionApplication(const RuleActionApplication &actionApp, QObject *parent = 0);
    explicit RuleActionApplication(QObject *parent = 0);

    RuleActionApplication &operator=(const RuleActionApplication &other);

    inline const QSet<QString> &getLaunchers() const { return _launchers; }
    void setLaunchers(const QSet<QString> &launchers);
    // For QML
    QVariantList getLaunchersQml() const;
    void setLaunchersQml(const QVariantList &uids);

    inline bool operator==(const RuleActionApplication &o) const {
        return this->_launchers == o._launchers;
    }
};

Q_DECLARE_METATYPE(RuleActionApplication *)
Q_DECLARE_METATYPE(RuleActionApplication)

QDBusArgument &operator <<(QDBusArgument &dbusArgument, const RuleActionApplication &obj);
const QDBusArgument &operator >>(const QDBusArgument &dbusArgument, RuleActionApplication &obj);

#endif // RULEACTIONAPPLICATION_H
