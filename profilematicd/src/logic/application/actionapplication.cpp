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
#include <QProcess>
#include <QDebug>

#include "actionapplication.h"

ActionApplication::ActionApplication()
{
}

void
ActionApplication::_launchApplication(const QString &launcher) const {
    // Would no doubt be something that should be handled by platform utils,
    // but change this if need be in the future
    QStringList args;
    args << launcher;

    IFDEBUG(qDebug() << "ActionApplication::launching" << launcher);
    bool success = QProcess::startDetached("/usr/bin/xdg-open", args);
    IFDEBUG(qDebug() << "ActionApplication::success " << success);
}

bool
ActionApplication::activateOnce(const Rule::IdType &, const RuleAction &action) {
    if (!action.application().getLaunchers().isEmpty()) {
        foreach (QString launcher, action.application().getLaunchers()) {
            _launchApplication(launcher);
        }
        return true;
    }

    return false;
}

void
ActionApplication::activateOnceExit(const Rule::IdType &, const RuleAction &action) {
    Q_UNUSED(action)
    // At least for now, nothing
}
