#include "Highwayman.h"

#include <iostream>

Highwayman::Highwayman()
    : Character(
        "Highwayman",
        {
            26, // max HP
            7,  // speed
            90, // accuracy
            4,  // min damage
            8,  // max damage
            10  // crit chance
        })
{
}

void Highwayman::pistolShot(Character& target)
{
    std::cout
        << name
        << " fires a Pistol Shot!\n";

    attack(target);
}

void Highwayman::meleeAttack(Character& target)
{
    std::cout
        << name
        << " uses a melee attack!\n";

    attack(target);
}