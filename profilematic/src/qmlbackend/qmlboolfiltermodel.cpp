#include "qmlboolfiltermodel.h"

QmlBoolFilterModel::QmlBoolFilterModel(QAbstractItemModel *sourceModel, int filterOnRole, bool filterCondition, QObject *parent)
    : QSortFilterProxyModel(parent), _filterOnRole(filterOnRole), _filterCondition(filterCondition)
{
    setSourceModel(sourceModel);
    setDynamicSortFilter(true);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(emitSizeChanged(QModelIndex,int,int)));
}

void
QmlBoolFilterModel::emitSizeChanged(const QModelIndex &, int, int) {
    // qDebug("QmlBoolFilterModel::emitSizeChanged");
    emit sizeChanged();
}

bool
QmlBoolFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const {
    // qDebug("QmlBoolFilterModel::filterAcceptsRow called");
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QVariant value = sourceModel()->data(index, _filterOnRole);

    return value.toBool() != _filterCondition;
}
