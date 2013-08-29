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
#ifndef TESTPMTIMER_H
#define TESTPMTIMER_H

#include <QtTest/QtTest>

#include "model/rule.h"

#include "../../src/util/pmtimer.h"

class TestPmTimer : public QObject
{
    Q_OBJECT


public:
    TestPmTimer();

private slots:
    void signalTest();

};

#endif // TESTPMTIMER_H
