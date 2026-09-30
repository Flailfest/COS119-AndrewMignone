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
void Highwayman::printSkills() const
{
    std::cout << "1. Pistol Shot\n";
    std::cout << "2. Melee Attack\n";
}

void Highwayman::useSkill(
    int skillNumber,
    Character& target
)
{
    switch (skillNumber)
    {
    case 1:
        pistolShot(target);
        break;

    case 2:
        meleeAttack(target);
        break;

    default:
        std::cout << "Invalid skill.\n";
        break;
    }
}