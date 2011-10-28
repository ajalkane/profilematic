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
#ifndef RULES_H
#define RULES_H

#include <QList>

#include "rule.h"

class RuleItem {
public:
    RuleItem();

    Rule rule;

    bool ruleActive;

    inline bool isTimeStartRuleUsable() const {
        return rule.getTimeStart().isValid();
    }
    inline bool isDaysRuleUsable() const {
        return !rule.getDays().isEmpty();
    }

    inline bool operator==(const RuleItem &o) const {
        return this->rule == o.rule
            && this->ruleActive == o.ruleActive;
    }
};

class Rules : public QList<RuleItem>
{
public:
    Rules();
};

#endif // RULES_H
