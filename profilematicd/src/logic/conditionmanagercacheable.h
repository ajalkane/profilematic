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
#ifndef CONDITIONMANAGERCACHEABLE_H
#define CONDITIONMANAGERCACHEABLE_H

#include <QObject>

#include "../model/rule.h"
#include "../model/rulecondition.h"

/**
 * Provides interface for ConditionManagers that can be cached. Classes implementing
 * this interface are supposed to return always the same result to match() function,
 * and if that changes call matchInvalidated(). Thus making match() cacheable.
 *
 * Eventually, concrete ConditionManagers implementations should be refactored to be
 * implementations of this interface when possible (for most it should be possible).
 *
 * Advantages to old interface:
 * - Possibility to cache matches to reduce cost of refreshes
 * - More straightforward implementation
 * - Unification of concepts
 *
 * The name of the class is not the best, but since currently only ConditionManagerCaching
 * manages implementations of this class, it is fitting.
 */
class ConditionManagerCacheable : public QObject
{
    Q_OBJECT

public:
    enum MatchStatus {
        MatchNotSet = -1,
        NotMatched,
        Matched
    };

    ConditionManagerCacheable(QObject *parent = 0);
    virtual ~ConditionManagerCacheable();

    /**
     * Returns true if the given condition has any rules that could match
     * in some potential situation. Practically this means that user
     * has specified the condition, ie. match returns something else
     * than "MatchNotSet"
     */
    virtual bool conditionSetForMatching(const RuleCondition &cond) const = 0;

    virtual MatchStatus match(const Rule::IdType &ruleId, const RuleCondition &cond) = 0;

    virtual void startMonitor() = 0;
    virtual void stopMonitor() = 0;

    /**
     * Called when one or more of the rules that have already been passed to match()
     * have been changed. Implementations should invalidate any data that has been
     * obtained from the RuleCondition's passed in match().
     */
    virtual void rulesChanged() = 0;

signals:
    // Implementing class must emit this if the state changes in a way that
    // match() function might give different results
    void matchInvalidated();
};

#endif // CONDITIONMANAGERCACHEABLE_H
