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
#ifndef TESTCONDITIONMANAGERCHAIN_H
#define TESTCONDITIONMANAGERCHAIN_H

#include <QList>
#include <QtTest/QtTest>

#include "model/rule.h"

#include "../../src/logic/conditionmanagerchain.h"

class TestConditionManagerChain: public QObject
{
    Q_OBJECT
public:
    TestConditionManagerChain();
private slots:
    void refresh_withNoMatches();
    void refresh_withMatches();

    void refreshNeeded_signalTest();
};

#endif // TESTCONDITIONMANAGERCHAIN_H
