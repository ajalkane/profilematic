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
#ifndef QMLNETWORKINFO_H
#define QMLNETWORKINFO_H

#include <QObject>
#include <QString>
#include <QNetworkConfigurationManager>
#include <QNetworkConfiguration>

/**
 * Compatibility class between Harmattan and QtMobility's NetworkInfo QML element.
 *
 * WARNING: only implements the subset of NetworkInfo that is needed by ProfileMatic.
 *
 * On Harmattan, QtMobility's NetworkInfo's name property returns different value
 * than QNetworkConfigurationManager's similar function. This doesn't cause normally
 * problems, but if user has renamed his Internet connection in Harmattan, then
 * problems arise.
 *
 * QNetworkConfigurationManager returns as name the same as user has edited in
 * Internet connection settings (default is what is obtained from WLAN AP). QtMobility's
 * NetworkInfo on the other always seems to return what is obtained from WLAN AP.
 *
 * This class implements QtMobility's NetworkInfo element using QNetworkConfigurationManager,
 * so that the name used by ProfileMatic backend is kept in sync with UI.
 */
class QmlNetworkInfo : public QObject
{
    Q_OBJECT
public:
    enum Mode {
        UnknownMode = 0,
        WlanMode = 4
    };

private:
    Q_ENUMS(Mode)

    Q_PROPERTY(enum Mode mode READ mode WRITE setPreferredMode NOTIFY modeChanged)
    Q_PROPERTY(QString networkName READ networkName)
    Q_PROPERTY(QString networkStatus READ networkStatus)

    Q_PROPERTY(bool monitorNameChanges READ monitorNameChanges WRITE setMonitorNameChanges)
    Q_PROPERTY(bool monitorModeChanges READ monitorModeChanges WRITE setMonitorModeChanges)
    Q_PROPERTY(bool monitorStatusChanges READ monitorStatusChanges WRITE setMonitorStatusChanges)

    QNetworkConfigurationManager *_networkConfigurationManager;

    bool _monitorNameChanges;
    bool _monitorModeChanges;
    bool _monitorStatusChanges;

    Mode _networkMode;
    Mode _preferredMode;
    QNetworkConfiguration _currentConfiguration;
    QString _networkStatus;

    QNetworkConfiguration _getCurrentActiveConfiguration() const;
    Mode _networkConfigurationToMode(const QNetworkConfiguration &nc);
    QString _networkConfigurationToStatus(const QNetworkConfiguration &nc);

public:
    QmlNetworkInfo(QObject *parent = 0);

    Mode mode() const;
    void setMode(const Mode mode);
    void setPreferredMode(const Mode mode);

    QString networkName() const;
    QString networkStatus() const;

    bool monitorNameChanges() const;
    void setMonitorNameChanges(bool monitor);

    bool monitorModeChanges() const;
    void setMonitorModeChanges(bool monitor);

    bool monitorStatusChanges() const;
    void setMonitorStatusChanges(bool monitor);

signals:
    void modeChanged();
    void nameChanged();
    void statusChanged();

private slots:
    void onConfigurationChanged(const QNetworkConfiguration &config);
};

#endif // QMLNETWORKINFO_H
