#include <QDebug>
#include <QFile>
#include <QDir>

#include "qmlapplicationsscanner.h"

namespace {
    const char *default_icon_paths[] = {
        "/usr/share/themes/blanco/meegotouch/icons/",
        "/usr/share/themes/base/meegotouch/icons/",

    };
}
#define array_sizeof(a) (int)(sizeof(a) / sizeof(a[0]))

#define ICON_PATH "/usr/share/themes/blanco/meegotouch/icons/"
#define ICON_PATH2 "/usr/share/themes/base/meegotouch/icons/"
#define DEFAULT_APP_ICON "file:///usr/share/themes/blanco/meegotouch/icons/icon-l-default-application.png"
#define APP_PATH_GLOBAL "/usr/share/applications/"

QmlApplicationsScanner::QmlApplicationsScanner(QObject *parent)
    : QObject(parent)
{
}

void
QmlApplicationsScanner::scan() {
    _scanFolder(APP_PATH_GLOBAL);
}

void
QmlApplicationsScanner::_scanFolder(const QString &path) {
    QDir dir(path);
    QStringList filter("*.desktop");
    QStringList files = dir.entryList(filter);
    for (int i = 0; i < files.count(); i++)
    {
        QString desktopPath = path + files.at(i);
        QmlApplication app = applicationFromLauncher(desktopPath);
        if (app.isUsable()) {
            emit applicationScanned(app.applicationName(), app.applicationLauncher(), app.iconUri());
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

    QString line = dfile.readLine();
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
                QString icon = "file://" + icon;
                app.setIconUri(icon);
            }
            //qDebug() << "Parsed icon: " << icon;
        }
        else if (line.startsWith("NotShowIn=X-MeeGo") ||
                 line.startsWith("OnlyShownIn=X-MeeGo"))
        {
            itemShownOnAppGrid = false;
        }
        line = dfile.readLine();
    } while (!line.isEmpty());

    dfile.close();

    if (app.isUsable() && itemShownOnAppGrid)
    {
        if (app.iconUri().size() == 0)
        {
            app.setIconUri(DEFAULT_APP_ICON);
        }
        return app;
    }

    return QmlApplication();
}
