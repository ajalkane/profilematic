#ifndef ACTIONFACTORY_H
#define ACTIONFACTORY_H

#include <QList>

#include "actionchain.h"

class ProfileClient;

class ActionFactory
{
    ActionFactory();
public:
    static ActionChain *create(ProfileClient *profileClient);
    static QList<Action*> createAsList(ProfileClient *profileClient);
};

#endif // ACTIONFACTORY_H
