#pragma once

#include "Character.h"

class Highwayman : public Character
{
public:
    Highwayman();

    void pistolShot(Character& target);
    void meleeAttack(Character& target);
};

