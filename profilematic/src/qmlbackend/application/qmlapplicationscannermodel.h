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

class QmlApplication {
    QString _applicationId;
    QString _applicationName;
    QString _iconUri;

public:
    QmlApplication();

    inline const QString& applicationId() const { return _applicationId; }
    inline const QString& applicationName() const { return _applicationName; }
    inline const QString& iconUri() const { return _iconUri; }

    inline void setApplicationId(const QString &applicationId) { _applicationId = applicationId; }
    inline void setApplicationName(const QString &applicationName) { _applicationName = applicationName; }
    inline void setIconUri(const QString &iconUri) { _iconUri = iconUri; }
};

class QmlApplicationScannerModel: public QAbstractListModel
{
    Q_OBJECT

    typedef QAbstractListModel super;

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

    QList<QmlApplication> _apps;

public:
    // IMPROVE: many of these roles are really not used anymore. Clean-up.
    enum ApplicationScannerRoles {
        ApplicationId = Qt::UserRole + 1,
        ApplicationName,
        IconUri
    };

    QmlApplicationScannerModel(QObject *parent = 0);

    inline int rowCount(const QModelIndex &parent = QModelIndex()) const { return _apps.count(); }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:
    void sizeChanged();
};

#endif // QMLAPPLICATIONSCANNERMODEL_H
