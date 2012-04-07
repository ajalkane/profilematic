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
#include "actionchain.h"

ActionChain::ActionChain()
{
}

ActionChain::~ActionChain()
{
    while (!_actions.isEmpty()) {
        Action *action = _actions.back();
        _actions.pop_back();
        delete action;
    }
}

void
ActionChain::startRefresh() {
    QList<Action *>::const_iterator i = _actions.constBegin();
    for (; i != _actions.constEnd(); ++i) {
        Action *action = *i;
        action->startRefresh();
    }
}

void
ActionChain::activate(const Rule::IdType &ruleId, const RuleAction &rule) {
    QList<Action *>::const_iterator i = _actions.constBegin();
    for (; i != _actions.constEnd(); ++i) {
        Action *action = *i;
        action->activate(ruleId, rule);
    }
}

void
ActionChain::endRefresh() {
    QList<Action *>::const_iterator i = _actions.constBegin();
    for (; i != _actions.constEnd(); ++i) {
        Action *action = *i;
        action->endRefresh();
    }
}

void
ActionChain::add(Action *action) {
    _actions.append(action);
}

