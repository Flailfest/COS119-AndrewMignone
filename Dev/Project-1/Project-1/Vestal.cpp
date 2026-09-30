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

void Vestal::attack(Character& target)
{
    if (!canUseFromPosition(1, 4))
    {
        std::cout << "Vestal cannot attack from this position.\n";
        return;
    }

    if (!isTargetInRange(target, 1, 2))
    {
        std::cout << "That target is out of range.\n";
        return;
    }

    Character::attack(target);
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
void Vestal::printSkills() const
{
    std::cout << "1. Smite\n";
    std::cout << "2. Heal Ally\n";
}

void Vestal::useSkill(
    int skillNumber,
    Character& target
)
{
    switch (skillNumber)
    {
    case 1:
        smite(target);
        break;

    case 2:
        healAlly(target);
        break;

    default:
        std::cout << "Invalid skill.\n";
        break;
    }
}
TargetType Vestal::getSkillTargetType(int skillNumber) const
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