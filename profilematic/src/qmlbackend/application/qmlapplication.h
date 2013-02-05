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
#ifndef QMLAPPLICATION_H
#define QMLAPPLICATION_H

#include <QString>

class QmlApplication {
    QString _applicationLauncher;
    QString _applicationName;
    QString _iconUri;

public:
    QmlApplication();
    QmlApplication(const QmlApplication &o);
    QmlApplication &operator=(const QmlApplication &o);

    inline const QString& applicationLauncher() const { return _applicationLauncher; }
    inline const QString& applicationName() const { return _applicationName; }
    inline const QString& iconUri() const { return _iconUri; }

    inline void setApplicationLauncher(const QString &applicationLauncher) { _applicationLauncher = applicationLauncher; }
    inline void setApplicationName(const QString &applicationName) { _applicationName = applicationName; }
    inline void setIconUri(const QString &iconUri) { _iconUri = iconUri; }

    inline bool isUsable() const {
        return !_applicationLauncher.isEmpty()
            && !_applicationName.isEmpty();
    }
};

#endif // QMLAPPLICATION_H
