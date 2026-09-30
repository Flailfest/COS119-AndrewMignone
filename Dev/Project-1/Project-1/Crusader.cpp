#include "Crusader.h"

#include <iostream>

Crusader::Crusader()
    : Character(
        "Crusader",
        {
            33, // max HP
            3,  // speed
            85, // accuracy
            5,  // min damage
            9,  // max damage
            5   // crit chance
        })
{
}

void Crusader::holyStrike(Character& target)
{
    std::cout
        << name
        << " uses Holy Strike!\n";

    attack(target);
}

void Crusader::healAlly(Character& target)
{
    std::cout
        << name
        << " uses Inspiring Heal on "
        << target.getName()
        << "!\n";

    target.heal(5);
}