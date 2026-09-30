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
void Crusader::attack(Character& target)
{
    if (!canUseFromPosition(1, 2))
    {
        std::cout
            << "Crusader cannot attack from this position.\n";

        return;
    }

    if (!isTargetInRange(target, 1, 2))
    {
        std::cout
            << "That target is out of range.\n";

        return;
    }

    Character::attack(target);
}

void Crusader::holyStrike(Character& target)
{
    if (!canUseFromPosition(1, 2))
    {
        std::cout
            << "Crusader cannot use Smite from this position.\n";

        return;
    }

    if (!isTargetInRange(target, 3, 4))
    {
        std::cout
            << "That target is out of range for Smite.\n";

        return;
    }

    attack(target);
}

void Crusader::healAlly(Character& target)
{
    if (!isTargetInRange(target, 1, 3))
    {
        std::cout
            << "That ally is out of range.\n";

        return;
    }

    target.heal(5);
}
void Crusader::printSkills() const
{
    std::cout << "1. Holy Strike\n";
    std::cout << "2. Heal Ally\n";
}

void Crusader::useSkill(
    int skillNumber,
    Character& target
)
{
    switch (skillNumber)
    {
    case 1:
        holyStrike(target);
        break;

    case 2:
        healAlly(target);
        break;

    default:
        std::cout << "Invalid skill.\n";
        break;
    }
}
TargetType Crusader::getSkillTargetType(int skillNumber) const
{
    switch (skillNumber)
    {
    case 1:
        return TargetType::Enemy;

    case 2:
        return TargetType::Ally;

    default:
        return TargetType::Enemy;
    }
}