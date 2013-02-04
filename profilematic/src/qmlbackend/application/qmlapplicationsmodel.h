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
#ifndef QMLAPPLICATIONSCANNERMODEL_H
#define QMLAPPLICATIONSCANNERMODEL_H

#include <QObject>
#include <QString>
#include <QAbstractListModel>

#include "qmlapplication.h"

class QmlApplicationsModel: public QAbstractListModel
{
    Q_OBJECT

    typedef QAbstractListModel super;

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

    QList<QmlApplication> _apps;

public:
    enum ApplicationScannerRoles {
        ApplicationLauncher = Qt::UserRole + 1,
        ApplicationName,
        IconUri
    };

    QmlApplicationsModel(QObject *parent = 0);

    inline int rowCount(const QModelIndex & = QModelIndex()) const { return _apps.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void addApplication(const QmlApplication &app);
    void clearApplications();

    Q_INVOKABLE inline QString getLauncher(int index) const {
        if (index >= 0 && index < _apps.count()) {
            return _apps.at(index).applicationLauncher();
        }
        return QString();
    }

signals:
    void sizeChanged();

private slots:
    void emitSizeChanged(const QModelIndex & parent, int start, int end);
};

#endif // QMLAPPLICATIONSCANNERMODEL_H
