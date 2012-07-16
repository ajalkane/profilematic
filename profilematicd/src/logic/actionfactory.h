#ifndef ACTIONFACTORY_H
#define ACTIONFACTORY_H

#include "action.h"

class ProfileClient;

class ActionFactory
{
    ActionFactory();
public:
    static Action *create(ProfileClient *profileClient);
};

#endif // ACTIONFACTORY_H
