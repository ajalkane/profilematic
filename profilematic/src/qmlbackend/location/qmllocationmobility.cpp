#include "qmllocationmobility.h"

QmlLocationMobility::QmlLocationMobility(QObject *parent)
    : QmlLocation(parent)
{
    _source = new QSystemNetworkInfo();
}

QmlLocationMobility::~QmlLocationMobility()
{
    delete _source;
}

int
QmlLocationMobility::getCurrentCell() const
{
    return _source->cellId();
}

void
QmlLocationMobility::monitorCellIdChange(bool monitor) {
    if (monitor) {
        connect(_source, SIGNAL(cellIdChanged(int)), this, SIGNAL(cellIdChanged(int)), Qt::UniqueConnection);
    } else {
        disconnect(_source, SIGNAL(cellIdChanged(int)), this, SIGNAL(cellIdChanged(int)));
    }
}
