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
#ifndef ACTION_H
#define ACTION_H

#include "model/ruleaction.h"
#include "model/rule.h"

#include "../util/conditionallogging.h"

class Action
{
public:
    Action();
    virtual ~Action();

    virtual void startRefresh() = 0;

    virtual void activate(const Rule::IdType &ruleId, const RuleAction &rule) = 0;

    virtual void endRefresh() = 0;

};

#endif // ACTION_H
