/**********************************************************************
 * Copyright 2011-2012 Philip Lorenz
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

#ifndef QMLACCOUNTSMODELSTUB_H
#define QMLACCOUNTSMODELSTUB_H

#include "qmlpresencemodel.h"

class QmlPresenceModelStub : public QmlPresenceModel
{
    Q_OBJECT
public:
    explicit QmlPresenceModelStub(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
private:
    struct StubAccountEntry : public AccountEntry {
        StubAccountEntry(int id,
                         QString name,
                         PresenceRule *rule,
                         QString icon = "icon-m-ovi-service-email",
                         bool enabled = true);

        int id;
        QString name;
        QString icon;
        bool enabled;
    };
private slots:
    void onRuleChanged();
    void addRandom();
    void removeRandom();
private:
    void appendRow(int id,
                   QString name,
                   QString icon = "icon-m-ovi-service-email",
                   bool enabled = true);
};

#endif // QMLACCOUNTSMODELSTUB_H
