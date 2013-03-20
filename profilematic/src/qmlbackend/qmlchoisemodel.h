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
#ifndef QMLCHOISEMODEL_H
#define QMLCHOISEMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

class QmlChoiseModel : public QAbstractListModel
{
    Q_OBJECT

    class ChoiseItem {
    public:
        int itemId;
        QString shortDescription;
        QString longDescription;

        ChoiseItem();
        ChoiseItem(const ChoiseItem &);
        inline ChoiseItem &operator=(const ChoiseItem &o) {
            itemId = o.itemId;
            shortDescription = o.shortDescription;
            longDescription = o.longDescription;
            return *this;
        }
    };

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

    QHash<int, QByteArray> _roleToProperty;

    QList<ChoiseItem> _choiseItems;

public:
    enum ChoiseRoles {
        IdRole = Qt::UserRole + 1,
        ShortDescriptionChoiseRole,
        LongDescriptionChoiseRole
    };

    QmlChoiseModel(QObject *parent = 0);

    // QAbstractListModel implementations
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE void add(int itemId, const QString &shortDesc, const QString &longDesc);
    Q_INVOKABLE QVariantMap get(int row);

public slots:
    void emitSizeChanged(const QModelIndex & parent, int start, int end);

signals:
    void sizeChanged();

};

#endif // QMLCHOISEMODEL_H
