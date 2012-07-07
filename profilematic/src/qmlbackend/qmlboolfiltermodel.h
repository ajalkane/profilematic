#ifndef QMLBOOLFILTERMODEL_H
#define QMLBOOLFILTERMODEL_H

#include <QSortFilterProxyModel>

class QmlBoolFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    int  _filterOnRole;
    bool _filterCondition;

    // Needed for SelectionDialog
    Q_PROPERTY(int count READ rowCount NOTIFY sizeChanged)

public:
    QmlBoolFilterModel(QAbstractItemModel *sourceModel, int filterOnRole, bool filterCondition, QObject *parent = 0);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private slots:
    void emitSizeChanged(const QModelIndex &, int, int);
signals:
    void sizeChanged();
};

#endif // QMLBOOLFILTERMODEL_H
