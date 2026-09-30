#pragma once

#include "Character.h"

class PlagueDoctor : public Character
{
public:
    PlagueDoctor();

    void plagueGrenade(Character& target);
    void healAlly(Character& target);
};

