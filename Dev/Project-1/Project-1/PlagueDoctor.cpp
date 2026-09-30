#include "PlagueDoctor.h"

#include <iostream>

PlagueDoctor::PlagueDoctor()
    : Character(
        "Plague Doctor",
        {
            22, // max HP
            6,  // speed
            85, // accuracy
            3,  // min damage
            6,  // max damage
            7   // crit chance
        })
{
}

void PlagueDoctor::plagueGrenade(Character& target)
{
    std::cout
        << name
        << " throws a Plague Grenade!\n";

    attack(target);
}

void PlagueDoctor::healAlly(Character& target)
{
    std::cout
        << name
        << " uses Battlefield Medicine on "
        << target.getName()
        << "!\n";

    target.heal(4);
}