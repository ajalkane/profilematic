#ifndef CONDITIONMANAGERFACTORY_H
#define CONDITIONMANAGERFACTORY_H

#include "conditionmanager.h"

class ConditionManagerFactory
{
    ConditionManagerFactory();
public:
    static ConditionManager *create();
};

#endif // CONDITIONMANAGERFACTORY_H
