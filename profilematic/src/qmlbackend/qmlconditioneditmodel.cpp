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

#include "qmlconditioneditmodel.h"

#include "qmlruleutil.h"

#define DESCRIPTION(name, topic, qmlFile, isDefault) \
    struct Description##name : public QmlBaseRuleEditModel::Description { \
        Description##name() : QmlConditionEditModel::Description(topic, qmlFile, isDefault) {} \
        virtual QString summary(const Rule &rule, bool inListing) const { \
            return summary(rule, "Not in use", inListing); \
        } \
\
        virtual QString summary(const Rule &rule, const QString &notUsed, bool inListing) const { \
            return QmlRuleUtil::instance()->name##Summary(&(rule.condition()), notUsed, inListing); \
        } \
\
        virtual bool isSet(const Rule &rule) const { \
            return !summary(rule, "", false).isEmpty(); \
        } \
        virtual void clear(Rule &rule) const { \
            QmlRuleUtil::instance()->name##Clear(&(rule.condition())); \
        } \
    } description##name;

DESCRIPTION(time, "Time", "ConditionTime.qml", true)
DESCRIPTION(cellLocation, "Cell id location", "ConditionLocation.qml", false)
DESCRIPTION(internetConnectionMode, "Internet connection", "ConditionInternetConnectionMode.qml", false)
DESCRIPTION(wlan, "WLAN", "ConditionWlan.qml", false)
DESCRIPTION(idle, "Idle", "ConditionIdle.qml", false)
DESCRIPTION(nfc,  "NFC", "ConditionNFC.qml", false)
DESCRIPTION(charging,  "Charging state", "ConditionChargingState.qml", false)
DESCRIPTION(batteryLevel,  "Battery level", "ConditionBatteryLevel.qml", false)

QmlConditionEditModel::QmlConditionEditModel(Rule *editRule, QObject *parent)
    : super(editRule, parent)
{
    _descriptions << &descriptiontime
                  << &descriptioncellLocation
                  << &descriptioninternetConnectionMode
                  << &descriptionwlan
                  << &descriptionidle
                  << &descriptionnfc
                  << &descriptioncharging
                  << &descriptionbatteryLevel;

    connect(_editRule, SIGNAL(conditionChanged()), this, SLOT(ruleChanged()));
}
