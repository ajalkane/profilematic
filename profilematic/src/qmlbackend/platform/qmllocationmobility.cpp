#include "qmllocationmobility.h"

QmlLocationMobility::QmlLocationMobility(QObject *parent)
    : QmlLocation(parent)
{
    _source = new QSystemNetworkInfo();
    connect(_source, SIGNAL(cellIdChanged(int)), this, SIGNAL(cellIdChanged(int)));
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
