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
void PlagueDoctor::printSkills() const
{
    std::cout << "1. Plague Grenade\n";
    std::cout << "2. Heal Ally\n";
}

void PlagueDoctor::useSkill(
    int skillNumber,
    Character& target
)
{
    switch (skillNumber)
    {
    case 1:
        plagueGrenade(target);
        break;

    case 2:
        healAlly(target);
        break;

    default:
        std::cout << "Invalid skill.\n";
        break;
    }
}