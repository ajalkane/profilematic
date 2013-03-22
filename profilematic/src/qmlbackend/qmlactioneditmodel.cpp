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

#include "qmlactioneditmodel.h"

#include "qmlruleutil.h"

#define DESCRIPTION(name, topic, qmlFile, isDefault) \
    struct Description##name : public QmlBaseRuleEditModel::Description { \
        Description##name() : QmlActionEditModel::Description(topic, qmlFile, isDefault) {} \
        virtual QString summary(const Rule &rule, bool inListing) const { \
            return summary(rule, tr(QT_TRANSLATE_NOOP("QmlActionEditModel", "Don't change")), inListing); \
        } \
\
        virtual QString summary(const Rule &rule, const QString &notUsed, bool inListing) const { \
            return QmlRuleUtil::instance()->name##Summary(&(rule.action()), notUsed, inListing); \
        } \
\
        virtual bool isSet(const Rule &rule) const { \
            return !summary(rule, "", false).isEmpty(); \
        } \
        virtual void clear(Rule &rule) const { \
            QmlRuleUtil::instance()->name##Clear(&(rule.action())); \
        } \
    }; \
    static Description##name description##name;

QmlActionEditModel::QmlActionEditModel(Rule *editRule, QObject *parent)
    : super(editRule, parent)
{
    DESCRIPTION(profile, tr("Profile"), "ActionProfile.qml", true)
    DESCRIPTION(presence, tr("Account availability"), "ActionPresence.qml", false)
    DESCRIPTION(flightMode, tr("Flight mode"), "ActionFlightMode.qml", false)
    DESCRIPTION(powerSavingMode, tr("Power saving mode"), "ActionPowerSavingMode.qml", false)
    DESCRIPTION(bluetoothMode, tr("Bluetooth"), "ActionBlueToothMode.qml", false)
    DESCRIPTION(cellularMode, tr("Mobile network mode"), "ActionCellularMode.qml", false)
    DESCRIPTION(standByScreenMode, tr("Stand-by screen mode"), "ActionStandByScreen.qml", false)
    DESCRIPTION(customAction, tr("Custom action"), "ActionCommandLine.qml", false)
    DESCRIPTION(application, tr("Application"), "ActionApplication.qml", false)
    DESCRIPTION(deviceVolume, tr("Device volume"), "ActionDeviceVolume.qml", false)
    DESCRIPTION(alarm, tr("Alarm"), "ActionAlarm.qml", false)

    _descriptions << &descriptionprofile
                  << &descriptionpresence
                  << &descriptionflightMode
                  << &descriptionpowerSavingMode
                  << &descriptionbluetoothMode
                  << &descriptioncellularMode
                  << &descriptionstandByScreenMode
                  << &descriptioncustomAction
                  << &descriptionapplication
                  << &descriptiondeviceVolume
                  << &descriptionalarm;

    connect(_editRule, SIGNAL(actionChanged()), this, SLOT(ruleChanged()));
}
