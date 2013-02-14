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
#include "ruleaction.h"

RuleAction::RuleAction(QObject *parent) : QObject(parent),
    _restoreProfile(false), _profileVolume(-1),
    _flightMode(-1), _restoreFlightMode(false), _powerSavingMode(-1), _restorePowerSavingMode(false),
    _blueToothMode(-1), _restoreBlueToothMode(false),
    _cellularMode(-1), _restoreCellularMode(false),
    _standByScreenMode(-1), _restoreStandByScreenMode(false),
    _backgroundConnectionsMode(-1), _restoreBackgroundConnectionsMode(false),
    _deviceVolume(-1),
    _restorePresence(false),
    _presenceChangeType(CustomPresenceType)
{
    _init();
}

RuleAction::RuleAction(const RuleAction &o)
    : QObject(NULL),
      _profile(o._profile),
      _restoreProfile(o._restoreProfile),
      _profileVolume(o._profileVolume),
      _flightMode(o._flightMode),
      _restoreFlightMode(o._restoreFlightMode),
      _powerSavingMode(o._powerSavingMode),
      _restorePowerSavingMode(o._restorePowerSavingMode),
      _blueToothMode(o._blueToothMode),
      _restoreBlueToothMode(o._restoreBlueToothMode),
      _cellularMode(o._cellularMode),
      _restoreCellularMode(o._restoreCellularMode),
      _commandLine(o._commandLine),
      _commandLineExit(o._commandLineExit),
      _standByScreenMode(o._standByScreenMode),
      _restoreStandByScreenMode(o._restoreStandByScreenMode),
      _backgroundConnectionsMode(o._backgroundConnectionsMode),
      _restoreBackgroundConnectionsMode(o._restoreBackgroundConnectionsMode),
      _application(o._application),
      _deviceVolume(o._deviceVolume),
      _alarm(o._alarm),
      _presenceStatusMessage(o._presenceStatusMessage),
      _restorePresence(o._restorePresence),
      _presenceChangeType(o._presenceChangeType)
{
    _init();
    // Copy the account rules
    setPresenceRules(o.presenceRules());
}

void
RuleAction::_init() {
    connect(this, SIGNAL(profileChanged()),                this, SIGNAL(changed()));
    connect(this, SIGNAL(restoreProfileChanged()),         this, SIGNAL(changed()));
    connect(this, SIGNAL(profileVolumeChanged()),          this, SIGNAL(changed()));
    connect(this, SIGNAL(flightModeChanged()),             this, SIGNAL(changed()));
    connect(this, SIGNAL(restoreFlightModeChanged()),      this, SIGNAL(changed()));
    connect(this, SIGNAL(powerSavingModeChanged()),        this, SIGNAL(changed()));
    connect(this, SIGNAL(restorePowerSavingModeChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(blueToothModeChanged()),          this, SIGNAL(changed()));
    connect(this, SIGNAL(restoreBlueToothModeChanged()),   this, SIGNAL(changed()));
    connect(this, SIGNAL(cellularModeChanged()),           this, SIGNAL(changed()));
    connect(this, SIGNAL(restoreCellularModeChanged()),    this, SIGNAL(changed()));
    connect(this, SIGNAL(commandLineChanged()),            this, SIGNAL(changed()));
    connect(this, SIGNAL(commandLineExitChanged()),            this, SIGNAL(changed()));
    connect(this, SIGNAL(presenceRulesChanged()),          this, SIGNAL(changed()));
    connect(this, SIGNAL(presenceStatusMessageChanged()),  this, SIGNAL(changed()));
    connect(this, SIGNAL(restorePresenceChanged()),        this, SIGNAL(changed()));
    connect(this, SIGNAL(presenceChangeTypeChanged()),     this, SIGNAL(changed()));
    connect(this, SIGNAL(standByScreenModeChanged()),      this, SIGNAL(changed()));
    connect(this, SIGNAL(restoreStandByScreenModeChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(backgroundConnectionsModeChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(restoreBackgroundConnectionsModeChanged()), this, SIGNAL(changed()));
    connect(&_application, SIGNAL(changed()),     this, SIGNAL(applicationChanged()));
    connect(this, SIGNAL(applicationChanged()),   this, SIGNAL(changed()));
    connect(this, SIGNAL(deviceVolumeChanged()),          this, SIGNAL(changed()));
    connect(&_alarm, SIGNAL(changed()),     this, SIGNAL(alarmChanged()));
    connect(this, SIGNAL(alarmChanged()),   this, SIGNAL(changed()));
}

RuleAction::~RuleAction() {}

RuleAction &
RuleAction::operator=(const RuleAction &o) {
    _profile = o._profile;
    _restoreProfile = o._restoreProfile;
    _profileVolume = o._profileVolume;
    _flightMode = o._flightMode;
    _restoreFlightMode = o._restoreFlightMode;
    _powerSavingMode = o._powerSavingMode;
    _restorePowerSavingMode = o._restorePowerSavingMode;
    _blueToothMode = o._blueToothMode;
    _restoreBlueToothMode = o._restoreBlueToothMode;
    _cellularMode = o._cellularMode;
    _restoreCellularMode = o._restoreCellularMode;
    _commandLine = o._commandLine;
    _commandLineExit = o._commandLineExit;
    _presenceStatusMessage = o._presenceStatusMessage;
    _restorePresence = o._restorePresence;
    _presenceChangeType = o._presenceChangeType;
    _standByScreenMode = o._standByScreenMode;
    _restoreStandByScreenMode = o._restoreStandByScreenMode;
    _backgroundConnectionsMode = o._backgroundConnectionsMode;
    _restoreBackgroundConnectionsMode = o._restoreBackgroundConnectionsMode;
    _application = o._application;
    _deviceVolume = o._deviceVolume;
    _alarm = o._alarm;

    setPresenceRules(o.presenceRules());
    return *this;
}

QString
RuleAction::getProfile() const {
    return _profile;
}

void
RuleAction::setProfile(const QString &profile) {
    if (_profile != profile) {
        _profile = profile;
        emit profileChanged();
    }
}

bool
RuleAction::getRestoreProfile() const {
    return _restoreProfile;
}

void
RuleAction::setRestoreProfile(bool restore) {
    if (_restoreProfile != restore) {
        _restoreProfile = restore;
        emit restoreProfileChanged();
    }
}

int
RuleAction::getProfileVolume() const {
    return _profileVolume;
}

void
RuleAction::setProfileVolume(int volume) {
    if (_profileVolume != volume) {
        _profileVolume = volume;
        emit profileVolumeChanged();
    }
}

QString
RuleAction::getCommandLine() const {
    return _commandLine;
}

void
RuleAction::setCommandLine(const QString &commandLine) {
    if (_commandLine != commandLine) {
        _commandLine = commandLine;
        if (_commandLine.trimmed().isEmpty()) {
            _commandLine.clear();
        }
        emit commandLineChanged();
    }
}

void
RuleAction::setCommandLineExit(const QString &commandLine) {
    if (_commandLineExit != commandLine) {
        _commandLineExit = commandLine;
        if (_commandLineExit.trimmed().isEmpty()) {
            _commandLineExit.clear();
        }
        emit commandLineExitChanged();
    }
}

QList<QObject *> RuleAction::presenceRulesQml() const
{
    QList<QObject *> result;
    foreach (PresenceRule *presenceRule, _presenceRules)
        result.append(presenceRule);
    return result;
}

bool RuleAction::addPresenceRule(PresenceRule *presenceRule)
{
    foreach (PresenceRule *existingPresenceRule, _presenceRules) {
        if (existingPresenceRule->key() == presenceRule->key())
            return false;
    }

    connect(presenceRule, SIGNAL(actionChanged()), SLOT(onPresenceRuleChanged()));

    presenceRule->setParent(this);
    _presenceRules.append(presenceRule);

    updatePresenceChangeType(presenceRule);

    emit presenceRulesChanged();

    return true;
}

bool RuleAction::removePresenceRule(PresenceRule *presenceRule)
{
    bool success = _presenceRules.removeOne(presenceRule);

    if (success)
        presenceRule->deleteLater();

    return success;
}

PresenceRule *RuleAction::presenceRule(const PresenceRule::Key &key)
{
    foreach (PresenceRule *presenceRule, _presenceRules)
        if (presenceRule->key() == key)
            return presenceRule;

    return NULL;
}

const QString &RuleAction::getPresenceStatusMessage() const
{
    return _presenceStatusMessage;
}

void RuleAction::setPresenceStatusMessage(const QString &pressenceStatusMessage)
{
    if (_presenceStatusMessage == pressenceStatusMessage)
        return;

    _presenceStatusMessage = pressenceStatusMessage;

    emit presenceStatusMessageChanged();
}


void RuleAction::updatePresenceChangeType(const PresenceRule *presenceRule) {
    if ((presenceRule->action() != PresenceRule::SetOffline
            && _presenceChangeType == RuleAction::AllOfflinePresenceType)
        || ((presenceRule->action() == PresenceRule::SetOffline || presenceRule->action() == PresenceRule::Retain)
            && _presenceChangeType == RuleAction::AllOnlinePresenceType))
        setPresenceChangeType(RuleAction::CustomPresenceType);
}

bool RuleAction::getRestorePresence() const
{
    return _restorePresence;
}

void RuleAction::setRestorePresence(bool restorePresence)
{
    if (_restorePresence == restorePresence)
        return;

    _restorePresence = restorePresence;

    emit restorePresenceChanged();
}

RuleAction::PresenceChangeType RuleAction::getPresenceChangeType() const
{
    return _presenceChangeType;
}

void RuleAction::setPresenceChangeType(RuleAction::PresenceChangeType presenceChangeType)
{
    if (presenceChangeType == _presenceChangeType)
        return;

    _presenceChangeType = presenceChangeType;

    foreach (PresenceRule *presenceRule, _presenceRules) {
        switch (_presenceChangeType) {
        case AllOnlinePresenceType:
            switch (presenceRule->action()) {
            case PresenceRule::SetOffline:
            case PresenceRule::Retain:
                presenceRule->setAction(PresenceRule::SetOnline);
                break;
            default:
                break;
            }
            break;
        case AllOfflinePresenceType:
            presenceRule->setAction(PresenceRule::SetOffline);
            break;
        default:
            break;
        }
    }

    emit presenceChangeTypeChanged();
}

QList<PresenceRule *> RuleAction::presenceRules() const
{
    return _presenceRules;
}

void RuleAction::setPresenceRules(QList<PresenceRule *> presenceRules)
{
    bool noPresenceChanges = true;
    qDeleteAll(_presenceRules);
    _presenceRules.clear();

    blockSignals(true);

    foreach (PresenceRule *presenceRule, presenceRules) {
        // Skip rules which enforce the default action
        if (presenceRule->action() != PresenceRule::Retain)
            noPresenceChanges = false;

        addPresenceRule(new PresenceRule(*presenceRule, this));
    }

    blockSignals(false);

    if (noPresenceChanges)
        setRestorePresence(false);

    emit presenceRulesChanged();
}

int
RuleAction::getFlightMode() const {
    return _flightMode;
}

void
RuleAction::setFlightMode(int flightMode) {
    if (_flightMode != flightMode) {
        _flightMode = flightMode;
        emit flightModeChanged();
    }
}

bool
RuleAction::getRestoreFlightMode() const {
    return _restoreFlightMode;
}

void
RuleAction::setRestoreFlightMode(bool restore) {
    if (_restoreFlightMode != restore) {
        _restoreFlightMode = restore;
        emit restoreFlightModeChanged();
    }
}

int
RuleAction::getPowerSavingMode() const {
    return _powerSavingMode;
}

void
RuleAction::setPowerSavingMode(int state) {
    if (_powerSavingMode != state) {
        _powerSavingMode = state;
        emit powerSavingModeChanged();
    }
}

bool
RuleAction::getRestorePowerSavingMode() const {
    return _restorePowerSavingMode;
}

void
RuleAction::setRestorePowerSavingMode(bool restore) {
    if (_restorePowerSavingMode != restore) {
        _restorePowerSavingMode = restore;
        emit restorePowerSavingModeChanged();
    }
}

int
RuleAction::getBlueToothMode() const {
    return _blueToothMode;
}

void
RuleAction::setBlueToothMode(int blueToothMode) {
    if (_blueToothMode != blueToothMode) {
        _blueToothMode = blueToothMode;
        emit blueToothModeChanged();
    }
}

bool
RuleAction::getRestoreBlueToothMode() const {
    return _restoreBlueToothMode;
}

void
RuleAction::setRestoreBlueToothMode(bool restore) {
    if (_restoreBlueToothMode != restore) {
        _restoreBlueToothMode = restore;
        emit restoreBlueToothModeChanged();
    }
}

int
RuleAction::getCellularMode() const {
    return _cellularMode;
}

void
RuleAction::setCellularMode(int mode) {
    if (_cellularMode != mode) {
        _cellularMode = mode;
        emit cellularModeChanged();
    }
}

void
RuleAction::setRestoreCellularMode(bool restore) {
    if (_restoreCellularMode != restore) {
        _restoreCellularMode = restore;
        emit restoreCellularModeChanged();
    }
}

int
RuleAction::getStandByScreenMode() const {
    return _standByScreenMode;
}

void
RuleAction::setStandByScreenMode(int mode) {
    if (_standByScreenMode != mode) {
        _standByScreenMode = mode;
        emit standByScreenModeChanged();
    }
}

bool
RuleAction::getRestoreStandByScreenMode() const {
    return _restoreStandByScreenMode;
}

void
RuleAction::setRestoreStandByScreenMode(bool restore) {
    if (_restoreStandByScreenMode != restore) {
        _restoreStandByScreenMode = restore;
        emit restoreStandByScreenModeChanged();
    }
}

int
RuleAction::getBackgroundConnectionsMode() const {
    return _backgroundConnectionsMode;
}

void
RuleAction::setBackgroundConnectionsMode(int mode) {
    if (_backgroundConnectionsMode != mode) {
        _backgroundConnectionsMode = mode;
        emit backgroundConnectionsModeChanged();
    }
}

bool
RuleAction::getRestoreBackgroundConnectionsMode() const {
    return _restoreBackgroundConnectionsMode;
}

void
RuleAction::setRestoreBackgroundConnectionsMode(bool restore) {
    if (_restoreBackgroundConnectionsMode != restore) {
        _restoreBackgroundConnectionsMode = restore;
        emit restoreBackgroundConnectionsModeChanged();
    }
}

int
RuleAction::getDeviceVolume() const {
    return _deviceVolume;
}

void
RuleAction::setDeviceVolume(int volume) {
    if (_deviceVolume != volume) {
        _deviceVolume = volume;
        emit deviceVolumeChanged();
    }
}

void RuleAction::onPresenceRuleChanged()
{
    PresenceRule *presenceRule = qobject_cast<PresenceRule *>(sender());

    emit changed();

    if (!presenceRule)
        return;

    if (_presenceChangeType == CustomPresenceType)
        return;

    updatePresenceChangeType(presenceRule);
}

QDBusArgument &operator<<(QDBusArgument &argument, const RuleAction &ruleAction)
{
    argument.beginStructure();
    argument << ruleAction.getProfile();
    argument << ruleAction.getRestoreProfile();
    argument << ruleAction.getProfileVolume();
    argument << ruleAction.getFlightMode();
    argument << ruleAction.getRestoreFlightMode();
    argument << ruleAction.getPowerSavingMode();
    argument << ruleAction.getRestorePowerSavingMode();
    argument << ruleAction.getBlueToothMode();
    argument << ruleAction.getRestoreBlueToothMode();
    argument << ruleAction.getCellularMode();
    argument << ruleAction.getRestoreCellularMode();
    argument << ruleAction.getStandByScreenMode();
    argument << ruleAction.getRestoreStandByScreenMode();
    argument << ruleAction.getBackgroundConnectionsMode();
    argument << ruleAction.getRestoreBackgroundConnectionsMode();
    argument << ruleAction.getCommandLine();
    argument << ruleAction.getCommandLineExit();
    QList<PresenceRule> presenceRules;
    foreach(PresenceRule *presenceRule, ruleAction.presenceRules())
        presenceRules.append(*presenceRule);
    argument << presenceRules;
    argument << ruleAction.getPresenceStatusMessage();
    argument << ruleAction.getRestorePresence();
    argument << int(ruleAction.getPresenceChangeType());
    argument << ruleAction.application();
    argument << ruleAction.getDeviceVolume();
    argument << ruleAction.alarm();
    argument.endStructure();
    return argument;
}

// Retrieve the MyStructure data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleAction &ruleAction)
{
    QString profile = ruleAction.getProfile();
    bool    restoreProfile = ruleAction.getRestoreProfile();
    int     profileVolume = ruleAction.getProfileVolume();
    int     flightMode = ruleAction.getFlightMode();
    bool    restoreFlightMode = ruleAction.getRestoreFlightMode();
    int     psmState = ruleAction.getPowerSavingMode();
    bool    restorePsmState = ruleAction.getRestorePowerSavingMode();
    int     blueToothMode = ruleAction.getBlueToothMode();
    bool    restoreBlueToothMode = ruleAction.getRestoreBlueToothMode();
    int     cellularMode = ruleAction.getCellularMode();
    bool    restoreCellularMode = ruleAction.getRestoreCellularMode();
    int     standByScreenMode = ruleAction.getStandByScreenMode();
    bool    restoreStandByScreenMode = ruleAction.getRestoreStandByScreenMode();
    int     backgroundConnectionsMode = ruleAction.getBackgroundConnectionsMode();
    bool    restoreBackgroundConnectionsMode = ruleAction.getBackgroundConnectionsMode();
    QString commandLine = ruleAction.getCommandLine();
    QString commandLineExit = ruleAction.getCommandLineExit();
    QList<PresenceRule> presenceRules;
    QString presenceStatusMessage;
    bool restorePresence;
    int presenceChangeType;
    RuleActionApplication application;
    int     deviceVolume = ruleAction.getDeviceVolume();
    RuleActionAlarm alarm;

    argument.beginStructure();
    argument >> profile;
    argument >> restoreProfile;
    argument >> profileVolume;
    argument >> flightMode;
    argument >> restoreFlightMode;
    argument >> psmState;
    argument >> restorePsmState;
    argument >> blueToothMode;
    argument >> restoreBlueToothMode;
    argument >> cellularMode;
    argument >> restoreCellularMode;
    argument >> standByScreenMode;
    argument >> restoreStandByScreenMode;
    argument >> backgroundConnectionsMode;
    argument >> restoreBackgroundConnectionsMode;
    argument >> commandLine;
    argument >> commandLineExit;
    argument >> presenceRules;
    argument >> presenceStatusMessage;
    argument >> restorePresence;
    argument >> presenceChangeType;
    argument >> application;
    argument >> deviceVolume;
    argument >> alarm;
    argument.endStructure();

    ruleAction.setProfile(profile);
    ruleAction.setRestoreProfile(restoreProfile);
    ruleAction.setProfileVolume(profileVolume);
    ruleAction.setFlightMode(flightMode);
    ruleAction.setRestoreFlightMode(restoreFlightMode);
    ruleAction.setPowerSavingMode(psmState);
    ruleAction.setRestorePowerSavingMode(restorePsmState);
    ruleAction.setBlueToothMode(blueToothMode);
    ruleAction.setRestoreBlueToothMode(restoreBlueToothMode);
    ruleAction.setCellularMode(cellularMode);
    ruleAction.setRestoreCellularMode(restoreCellularMode);
    ruleAction.setStandByScreenMode(standByScreenMode);
    ruleAction.setRestoreStandByScreenMode(restoreStandByScreenMode);
    ruleAction.setBackgroundConnectionsMode(backgroundConnectionsMode);
    ruleAction.setRestoreBackgroundConnectionsMode(restoreBackgroundConnectionsMode);
    ruleAction.setCommandLine(commandLine);
    ruleAction.setCommandLineExit(commandLineExit);
    ruleAction.setApplication(application);
    ruleAction.setDeviceVolume(deviceVolume);
    ruleAction.setAlarm(alarm);

    QList<PresenceRule *> dstPresenceRules;
    foreach(const PresenceRule &presenceRule, presenceRules)
        dstPresenceRules.append(new PresenceRule(presenceRule));
    ruleAction.setPresenceRules(dstPresenceRules);

    qDeleteAll(dstPresenceRules);
    dstPresenceRules.clear();

    ruleAction.setPresenceStatusMessage(presenceStatusMessage);
    ruleAction.setRestorePresence(restorePresence);
    ruleAction.setPresenceChangeType((RuleAction::PresenceChangeType) presenceChangeType);

    return argument;
}
