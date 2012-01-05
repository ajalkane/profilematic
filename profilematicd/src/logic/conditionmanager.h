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
#ifndef CONDITIONMANAGER_H
#define CONDITIONMANAGER_H

#include <QObject>
#include <QList>

#include "../model/rule.h"

class ConditionManager : public QObject
{
    Q_OBJECT

protected:
    QList<Rule> _matchingRules;

public:
    ConditionManager();
    virtual ~ConditionManager();

    virtual void startRefresh();
    virtual void refresh(const QList<Rule> &rules) = 0;
    virtual void endRefresh();

    // Cleared when endRefresh() is called.
    // IMPROVE: keeping a copy of each matching rule for all
    // conditions uses unnecessarily memory.
    inline const QList<Rule> &matchingRules() {
        return _matchingRules;
    }

signals:
    // Signal sent when condition needs refreshing. When emitting this,
    void refreshNeeded();
};

#endif // CONDITIONMANAGER_H
