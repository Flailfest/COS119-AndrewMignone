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
void Highwayman::attack(Character& target)
{
    if (!canUseFromPosition(1, 3))
    {
        std::cout << "Highwayman cannot attack from this position.\n";
        return;
    }

    if (!isTargetInRange(target, 2, 4))
    {
        std::cout << "That target is out of range.\n";
        return;
    }

    Character::attack(target);
}

void Highwayman::pistolShot(Character& target)
{
    if (!canUseFromPosition(1, 3))
    {
        std::cout
            << "Highwayman cannot use Pistol Shot from this position.\n";

        return;
    }

    if (!isTargetInRange(target, 2, 4))
    {
        std::cout
            << "That target is out of range for Pistol Shot.\n";

        return;
    }

    attack(target);
}

void Highwayman::meleeAttack(Character& target)
{
    if (!canUseFromPosition(1, 2))
    {
        std::cout
            << "Highwayman cannot use Melee Attack from this position.\n";

        return;
    }

    if (!isTargetInRange(target, 1, 2))
    {
        std::cout
            << "That target is out of range for Melee Attack.\n";

        return;
    }

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