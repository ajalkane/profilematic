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
            return summary(rule, tr(QT_TRANSLATE_NOOP("QmlConditionEditModel", "Not in use")), inListing); \
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
    }; \
    static Description##name description##name;

QmlConditionEditModel::QmlConditionEditModel(Rule *editRule, QObject *parent)
    : super(editRule, parent)
{
    DESCRIPTION(time, tr("Time"), "ConditionTime.qml", true)
    DESCRIPTION(cellLocation, tr("Cell id location"), "ConditionLocation.qml", false)
    DESCRIPTION(internetConnectionMode, tr("Internet connection"), "ConditionInternetConnectionMode.qml", false)
    DESCRIPTION(wlan, tr("WLAN"), "ConditionWlan.qml", false)
    DESCRIPTION(idle, tr("Idle"), "ConditionIdle.qml", false)
    DESCRIPTION(nfc,  tr("NFC"), "ConditionNFC.qml", false)
    DESCRIPTION(charging,  tr("Charging state"), "ConditionChargingState.qml", false)
    DESCRIPTION(batteryLevel,  tr("Battery level"), "ConditionBatteryLevel.qml", false)
    DESCRIPTION(calendar,  tr("Calendar"), "ConditionCalendar.qml", false)

    _descriptions << &descriptiontime
                  << &descriptioncellLocation
                  << &descriptioninternetConnectionMode
                  << &descriptionwlan
                  << &descriptionidle
                  << &descriptionnfc
                  << &descriptioncharging
                  << &descriptionbatteryLevel
                  << &descriptioncalendar;

    connect(_editRule, SIGNAL(conditionChanged()), this, SLOT(ruleChanged()));
}
