#include "qmlapplicationsmodel.h"

QmlApplicationsModel::QmlApplicationsModel(QObject *parent)
    : super(parent)
{
    QHash<int, QByteArray> roleToProperty;

    roleToProperty[ApplicationLauncher] = "launcher";
    roleToProperty[ApplicationName]     = "name";
    roleToProperty[IconUri]             = "iconuri";

    setRoleNames(roleToProperty);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
}

void
QmlApplicationsModel::emitSizeChanged(const QModelIndex &, int, int) {
    emit sizeChanged();
}

QVariant
QmlApplicationsModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= _apps.count())
        return QVariant();

    const QmlApplication &app = _apps.at(index.row());
    switch (role) {
    case ApplicationLauncher:
        return app.applicationLauncher();
    case ApplicationName:
        return app.applicationName();
    case IconUri:
        return app.iconUri();
    }
    return QVariant();
}

void
QmlApplicationsModel::addApplication(const QmlApplication &app) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    _apps << app;
    endInsertRows();
}

void
QmlApplicationsModel::clearApplications() {
    beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    _apps.clear();
    endRemoveRows();
}

