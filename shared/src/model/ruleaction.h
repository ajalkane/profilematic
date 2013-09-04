#ifndef RULEACTION_H
#define RULEACTION_H

#include <QList>
#include <QMetaType>
#include <QDBusArgument>

#include "presencerule.h"
#include "ruleactionapplication.h"
#include "ruleactionalarm.h"

class RuleAction : public QObject
{
    Q_OBJECT

public:
    enum PresenceChangeType {
        CustomPresenceType,
        AllOnlinePresenceType,
        AllOfflinePresenceType
    };
private:
    Q_ENUMS(PresenceChangeType)

    // Actions
    QString _profile;
    bool    _restoreProfile;
    int     _profileVolume;
    int     _flightMode;
    bool    _restoreFlightMode;
    int     _powerSavingMode;
    bool    _restorePowerSavingMode;
    int     _blueToothMode;
    bool    _restoreBlueToothMode;
    int     _cellularMode;
    bool    _restoreCellularMode;
    QString _commandLine;
    QString _commandLineExit;
    int     _standByScreenMode;
    bool    _restoreStandByScreenMode;
    int     _backgroundConnectionsMode;
    bool    _restoreBackgroundConnectionsMode;
    RuleActionApplication _application;
    int     _deviceVolume;
    bool    _restoreDeviceVolume;
    RuleActionAlarm _alarm;

    QList<PresenceRule *> _presenceRules;
    QString _presenceStatusMessage;
    bool    _restorePresence;
    PresenceChangeType _presenceChangeType;

    int     _deviceBrightness;
    bool    _restoreDeviceBrightness;

    // IMPROVE: maybe the QML specifics could be in inheriting class, keeping this
    // class "pure" plain Qt object?
    Q_PROPERTY(QString profile READ getProfile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(bool restoreProfile READ getRestoreProfile WRITE setRestoreProfile NOTIFY restoreProfileChanged)
    Q_PROPERTY(int profileVolume READ getProfileVolume WRITE setProfileVolume NOTIFY profileVolumeChanged)
    Q_PROPERTY(int flightMode READ getFlightMode WRITE setFlightMode NOTIFY flightModeChanged)
    Q_PROPERTY(bool restoreFlightMode READ getRestoreFlightMode WRITE setRestoreFlightMode NOTIFY restoreFlightModeChanged)
    Q_PROPERTY(int powerSavingMode READ getPowerSavingMode WRITE setPowerSavingMode NOTIFY powerSavingModeChanged)
    Q_PROPERTY(bool restorePowerSavingMode READ getRestorePowerSavingMode WRITE setRestorePowerSavingMode NOTIFY restorePowerSavingModeChanged)
    Q_PROPERTY(int blueToothMode READ getBlueToothMode WRITE setBlueToothMode NOTIFY blueToothModeChanged)
    Q_PROPERTY(bool restoreBlueToothMode READ getRestoreBlueToothMode WRITE setRestoreBlueToothMode NOTIFY restoreBlueToothModeChanged)
    Q_PROPERTY(int cellularMode READ getCellularMode WRITE setCellularMode NOTIFY cellularModeChanged)
    Q_PROPERTY(bool restoreCellularMode READ getRestoreCellularMode WRITE setRestoreCellularMode NOTIFY restoreCellularModeChanged)
    Q_PROPERTY(QString commandLine READ getCommandLine WRITE setCommandLine NOTIFY commandLineChanged)
    Q_PROPERTY(QString commandLineExit READ getCommandLineExit WRITE setCommandLineExit NOTIFY commandLineExitChanged)
    Q_PROPERTY(int standByScreenMode READ getStandByScreenMode WRITE setStandByScreenMode NOTIFY standByScreenModeChanged)
    Q_PROPERTY(bool restoreStandByScreenMode READ getRestoreStandByScreenMode WRITE setRestoreStandByScreenMode NOTIFY restoreStandByScreenModeChanged)
    Q_PROPERTY(int backgroundConnectionsMode READ getBackgroundConnectionsMode WRITE setBackgroundConnectionsMode NOTIFY backgroundConnectionsModeChanged)
    Q_PROPERTY(bool restoreBackgroundConnectionsMode READ getRestoreBackgroundConnectionsMode WRITE setRestoreBackgroundConnectionsMode NOTIFY restoreBackgroundConnectionsModeChanged)
    Q_PROPERTY(RuleActionApplication *application READ getApplicationQml NOTIFY applicationChanged STORED false)
    Q_PROPERTY(int deviceVolume READ getDeviceVolume WRITE setDeviceVolume NOTIFY deviceVolumeChanged)
    Q_PROPERTY(bool restoreDeviceVolume READ getRestoreDeviceVolume WRITE setRestoreDeviceVolume NOTIFY restoreDeviceVolumeChanged)
    Q_PROPERTY(RuleActionAlarm *alarm READ getAlarmQml NOTIFY alarmChanged STORED false)
    Q_PROPERTY(int deviceBrightness READ getDeviceBrightness WRITE setDeviceBrightness NOTIFY deviceBrightnessChanged)
    Q_PROPERTY(bool restoreDeviceBrightness READ getRestoreDeviceBrightness WRITE setRestoreDeviceBrightness NOTIFY restoreDeviceBrightnessChanged)

    /**
      * This property gives access to the presence rules associated with this
      * rule.
      *
      * \note The setter copies the individual rule instances and binds them
      *       to the Rule instance.
      */
    Q_PROPERTY(QList<QObject *> presenceRules READ presenceRulesQml NOTIFY presenceRulesChanged)
    /**
      * This property represents the global status message set for all accounts
      * whose presence is changed to online by this rule. Specific online
      * status messages for each account can be specified in the corresponding
      * PresenceRule instance.
      */
    Q_PROPERTY(QString presenceStatusMessage READ getPresenceStatusMessage WRITE setPresenceStatusMessage NOTIFY presenceStatusMessageChanged)
    Q_PROPERTY(bool restorePresence READ getRestorePresence WRITE setRestorePresence NOTIFY restorePresenceChanged)
    Q_PROPERTY(PresenceChangeType presenceChangeType READ getPresenceChangeType WRITE setPresenceChangeType NOTIFY presenceChangeTypeChanged)

    void _init();
signals:
    void changed();

    void profileChanged();
    void restoreProfileChanged();
    void profileVolumeChanged();
    void flightModeChanged();
    void restoreFlightModeChanged();
    void powerSavingModeChanged();
    void restorePowerSavingModeChanged();
    void blueToothModeChanged();
    void restoreBlueToothModeChanged();
    void cellularModeChanged();
    void restoreCellularModeChanged();
    void commandLineChanged();
    void commandLineExitChanged();
    void presenceRulesChanged();
    void presenceStatusMessageChanged();
    void restorePresenceChanged();
    void presenceChangeTypeChanged();
    void standByScreenModeChanged();
    void restoreStandByScreenModeChanged();
    void backgroundConnectionsModeChanged();
    void restoreBackgroundConnectionsModeChanged();
    void nfcChanged();
    void applicationChanged();
    void deviceVolumeChanged();
    void restoreDeviceVolumeChanged();
    void alarmChanged();
    void deviceBrightnessChanged();
    void restoreDeviceBrightnessChanged();

private slots:
    void onPresenceRuleChanged();
public:
    typedef QString IdType;

public:
    RuleAction(QObject *parent = 0);
    RuleAction(const RuleAction &o);

    virtual ~RuleAction();

    RuleAction &operator=(const RuleAction &o);

    QString getProfile() const;
    void setProfile(const QString &profile);

    bool getRestoreProfile() const;
    void setRestoreProfile(bool restore);

    // Returns -1 if the profile volume is not to be set
    int getProfileVolume() const;
    // Use -1 if the profile volume is not to be set, otherwise value between 0 to 100
    void setProfileVolume(int volume);

    // -1 don't set, 0, set no flight mode, 1 set flight mode
    int getFlightMode() const;
    void setFlightMode(int mode);
    bool getRestoreFlightMode() const;
    void setRestoreFlightMode(bool restore);

    // -1 don't set, 0, set no power saving mode, 1 set power saving mode
    int getPowerSavingMode() const;
    void setPowerSavingMode(int state);
    bool getRestorePowerSavingMode() const;
    void setRestorePowerSavingMode(bool restore);

    // -1 don't set, 0 set BlueTooth off, 1 set BlueTooth on, 2 set BlueTooth on and visible
    int getBlueToothMode() const;
    void setBlueToothMode(int mode);
    bool getRestoreBlueToothMode() const;
    void setRestoreBlueToothMode(bool restore);

    // -1 don't set, 0 set Dual, 1 set 2G, 2 set 3G
    int getCellularMode() const;
    void setCellularMode(int mode);
    inline bool getRestoreCellularMode() const { return _restoreCellularMode; }
    void setRestoreCellularMode(bool restore);

    // -1 don't set, 0 set StandByScreen off, 1 set StandByScreen on
    int getStandByScreenMode() const;
    void setStandByScreenMode(int mode);
    bool getRestoreStandByScreenMode() const;
    void setRestoreStandByScreenMode(bool restore);

    // -1 don't set, 0 set backgroundConnections off, 1 set backgroundConnections on
    int getBackgroundConnectionsMode() const;
    void setBackgroundConnectionsMode(int mode);
    bool getRestoreBackgroundConnectionsMode() const;
    void setRestoreBackgroundConnectionsMode(bool restore);

    QString getCommandLine() const;
    void setCommandLine(const QString &commandLine);

    inline QString getCommandLineExit() const { return _commandLineExit; }
    void setCommandLineExit(const QString &commandLine);

    QList<QObject *> presenceRulesQml() const;

    QList<PresenceRule *> presenceRules() const;
    void setPresenceRules(QList<PresenceRule *> presenceRules);

    bool addPresenceRule(PresenceRule *presenceRule);
    bool removePresenceRule(PresenceRule *presenceRule);
    PresenceRule *presenceRule(const PresenceRule::Key &key);

    const QString &getPresenceStatusMessage() const;
    void setPresenceStatusMessage(const QString &pressenceStatusMessage);
    void updatePresenceChangeType(const PresenceRule *presenceRule);

    bool getRestorePresence() const;
    void setRestorePresence(bool restorePresence);

    PresenceChangeType getPresenceChangeType() const;
    void setPresenceChangeType(PresenceChangeType presenceChangeType);

    inline const RuleActionApplication &application() const { return _application; }
    inline RuleActionApplication &application() { return _application; }
    inline void setApplication(const RuleActionApplication &app) { _application = app; }
    // For QML
    inline RuleActionApplication *getApplicationQml() { return &_application; }

    // Returns -1 if the devcie volume is not to be set
    int getDeviceVolume() const;
    // Use -1 if the profile volume is not to be set, otherwise value between 0 to 100
    void setDeviceVolume(int volume);
    inline bool getRestoreDeviceVolume() const { return _restoreDeviceVolume; }
    void setRestoreDeviceVolume(bool restore);

    inline const RuleActionAlarm &alarm() const { return _alarm; }
    inline RuleActionAlarm &alarm() { return _alarm; }
    inline void setAlarm(const RuleActionAlarm &alarm) { _alarm = alarm; }
    // For QML
    inline RuleActionAlarm *getAlarmQml() { return &_alarm; }

    // -1 don't set, 0 -  100 (min - max depending on platform in percents)
    inline int getDeviceBrightness() const { return _deviceBrightness; }
    void setDeviceBrightness(int brightness);
    inline bool getRestoreDeviceBrightness() const { return _restoreDeviceBrightness; }
    void setRestoreDeviceBrightness(bool restore);

    inline bool operator==(const RuleAction &o) const {
        return this->_profile   == o._profile
            && this->_restoreProfile == o._restoreProfile
            && this->_profileVolume == o._profileVolume
            && this->_flightMode == o._flightMode
            && this->_restoreFlightMode == o._restoreFlightMode
            && this->_powerSavingMode == o._powerSavingMode
            && this->_restorePowerSavingMode == o._restorePowerSavingMode
            && this->_blueToothMode == o._blueToothMode
            && this->_restoreBlueToothMode == o._restoreBlueToothMode
            && this->_cellularMode == o._cellularMode
            && this->_restoreCellularMode == o._restoreCellularMode
            && this->_commandLine == o._commandLine
            && this->_commandLineExit == o._commandLineExit
            && this->_presenceStatusMessage == o._presenceStatusMessage
            && this->_restorePresence == o._restorePresence
            && this->_standByScreenMode == o._standByScreenMode
            && this->_restoreStandByScreenMode == o._restoreStandByScreenMode
            && this->_backgroundConnectionsMode == o._backgroundConnectionsMode
            && this->_restoreBackgroundConnectionsMode == o._restoreBackgroundConnectionsMode
            && this->_application == o._application
            && this->_deviceVolume == o._deviceVolume
            && this->_restoreDeviceVolume == o._restoreDeviceVolume
            && this->_alarm == o._alarm
            && this->_deviceBrightness == o._deviceBrightness
            && this->_restoreDeviceBrightness == o._restoreDeviceBrightness;

    }

};

Q_DECLARE_METATYPE(RuleAction)

// Marshall the Rule data into a D-Bus argument
QDBusArgument &operator<<(QDBusArgument &argument, const RuleAction &mystruct);

// Retrieve the Rule data from the D-Bus argument
const QDBusArgument &operator>>(const QDBusArgument &argument, RuleAction &mystruct);

#endif // RULEACTION_H
