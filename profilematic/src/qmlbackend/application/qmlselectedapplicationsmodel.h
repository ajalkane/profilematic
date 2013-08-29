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
#ifndef QMLSELECTEDAPPLICATIONSMODEL_H
#define QMLSELECTEDAPPLICATIONSMODEL_H

#include <QObject>
#include <QString>
#include <QAbstractListModel>
#include <QHash>
#include <QDebug>

#include "model/rule.h"

#include "qmlapplication.h"
#include "qmlapplicationsscanner.h"

class QmlSelectedApplicationsModel : public QAbstractListModel
{
    Q_OBJECT

    typedef QAbstractListModel super;

    Rule *_editRule;
    QmlApplicationsScanner *_scanner;
    QList<QString> _launchers;
    // Mutable because this is basically like a cache... data() requests
    // information from this cache, and this cache is updated if it
    // has not already been filled.
    mutable QHash<QString, QmlApplication> _launcherToApp;

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

    void _initFromEditRule();

    const QString &_nameForLauncher(const QString &launcher) const;
    const QString &_iconForLauncher(const QString &launcher) const;

    const QmlApplication &_appForLauncher(const QString &launcher) const;
protected:

public:
    enum Roles {
        ApplicationLauncher = Qt::UserRole + 1,
        ApplicationName,
        IconUri
    };

    QmlSelectedApplicationsModel(Rule *rule, QmlApplicationsScanner *_scanner, QObject *parent = 0);

    inline int rowCount(const QModelIndex & = QModelIndex()) const {
        // qDebug() << "QmlSelectedApplicationsModel rowCount called " << _lau
        return _launchers.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void initializeEdit();

signals:
    void sizeChanged();

private slots:
    void _launchersChanged();
};

#endif // QMLSELECTEDAPPLICATIONSMODEL_H
