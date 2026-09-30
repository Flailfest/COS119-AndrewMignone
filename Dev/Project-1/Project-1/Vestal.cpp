#include "Vestal.h"

#include <iostream>

Vestal::Vestal()
    : Character(
        "Vestal",
        {
            24, // max HP
            4,  // speed
            85, // accuracy
            3,  // min damage
            6,  // max damage
            5   // crit chance
        })
{
}

void Vestal::healAlly(Character& target)
{
    std::cout
        << name
        << " uses Divine Heal on "
        << target.getName()
        << "!\n";

    target.heal(8);
}

void Vestal::smite(Character& target)
{
    std::cout
        << name
        << " uses Smite!\n";

    attack(target);
}