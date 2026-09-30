#pragma once

#include "Character.h"

class Vestal : public Character
{
public:
    Vestal();

    void healAlly(Character& target);
    void smite(Character& target);
};

