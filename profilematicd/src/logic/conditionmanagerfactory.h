#ifndef CONDITIONMANAGERFACTORY_H
#define CONDITIONMANAGERFACTORY_H

#include <QList>

#include "conditionmanagerchain.h"

class ConditionManagerFactory
{
    ConditionManagerFactory();
public:
    static QList<ConditionManager *> createAsList();
};

#endif // CONDITIONMANAGERFACTORY_H
