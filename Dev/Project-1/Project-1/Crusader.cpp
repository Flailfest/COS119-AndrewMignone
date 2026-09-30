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