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
#include <QDebug>
#include <QFile>
#include <QDir>

#include "qmlapplicationsscanner.h"

namespace {
    const char *default_icon_paths[] = {
        "/usr/share/icons/hicolor/64x64/apps/",
        "/usr/share/themes/blanco/meegotouch/icons/",
        "/usr/share/themes/base/meegotouch/icons/",
    };
}
#define array_sizeof(a) (int)(sizeof(a) / sizeof(a[0]))

#define ICON_PATH "/usr/share/themes/blanco/meegotouch/icons/"
#define ICON_PATH2 "/usr/share/themes/base/meegotouch/icons/"
#define DEFAULT_APP_ICON "file:///usr/share/themes/blanco/meegotouch/icons/icon-l-default-application.png"
#define APP_PATH_GLOBAL "/usr/share/applications/"
#define APP_PATH_LOCAL "/home/user/.local/share/applications/"

QmlApplicationsScanner::QmlApplicationsScanner(QObject *parent)
    : QObject(parent)
{
}

void
QmlApplicationsScanner::scan() {
    _scanFolder(APP_PATH_GLOBAL);
    _scanFolder(APP_PATH_LOCAL);
}

void
QmlApplicationsScanner::_scanFolder(const QString &path) {
    QDir dir(path);
    QStringList filter("*.desktop");
    QStringList files = dir.entryList(filter);
    for (int i = 0; i < files.count(); i++)
    {
        // Skip harmattan's quicklaunchers
        QString fileName = files.at(i);
        if (!fileName.startsWith("quicklaunchbar")) {
            QString desktopPath = path + files.at(i);
            QmlApplication app = applicationFromLauncher(desktopPath);
            if (app.isUsable()) {
                emit applicationScanned(app.applicationName(), app.applicationLauncher(), app.iconUri());
            }
        }
    }
}

QmlApplication
QmlApplicationsScanner::applicationFromLauncher(const QString &launcher) {
    /**
     * This piece of code has been mostly copied from sandstr1's MyMoves codebase.
     * Thanks to him for most of it.
     */
    QFile dfile(launcher);
    dfile.open(QIODevice::ReadOnly);

    QString line = QString::fromUtf8(dfile.readLine());
    bool itemShownOnAppGrid = true;
    QmlApplication app;
    app.setApplicationLauncher(launcher);
    do {
        if (line.startsWith("Name=")) {
            QString name = line.trimmed().mid(5);
            app.setApplicationName(name);
        }
        else if (line.startsWith("Icon=")) {
            // qDebug() << "Icon found: " << line;
            QString icon = line.trimmed().mid(5);
            if (!icon.contains("/")) {
                for (int defaultI = 0; defaultI < array_sizeof(default_icon_paths); ++defaultI) {
                    QString defaultIconPath(default_icon_paths[defaultI]);
                    if (QFile::exists(defaultIconPath + icon + ".png")) {
                        app.setIconUri("file://" + defaultIconPath + icon + ".png");
                        break;
                    }
                    else if (QFile::exists(defaultIconPath + icon + ".svg")) {
                        app.setIconUri("file://" + defaultIconPath + icon + ".svg");
                        break;
                    }
                }
            }
            else {
                app.setIconUri("file://" + icon);
            }
            //qDebug() << "Parsed icon: " << icon;
        }
        else if (line.startsWith("NotShowIn=X-MeeGo")) {
            itemShownOnAppGrid = false;
        }
        line = QString::fromUtf8(dfile.readLine());
    } while (!line.isEmpty());

    dfile.close();

    if (app.isUsable() && itemShownOnAppGrid) {
        if (app.iconUri().size() == 0) {
            app.setIconUri(DEFAULT_APP_ICON);
        }
        return app;
    }

    return QmlApplication();
}
