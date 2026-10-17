#include "Arbalest.h"

#include <iostream>

Arbalest::Arbalest()
    : Character("Arbalest", { 27, 3, 85, 7, 12, 8 })
{
}

void Arbalest::printSkills()
{
    printSkillList();
}

void Arbalest::printSkills() const
{
    printSkillList();
}

void Arbalest::printSkillList() const
{
    std::cout << "1. Sniper Shot (heavy ranged damage)\n";
    std::cout << "2. Battlefield Bandage (heal an ally for 4 HP)\n";
}

void Arbalest::useSkill(int skillNumber, Character& target)
{
    if (skillNumber == 1)
    {
        sniperShot(target);
    }
    else if (skillNumber == 2)
    {
        battlefieldBandage(target);
    }
}

TargetType Arbalest::getSkillTargetType(int skillNumber)
{
    return static_cast<const Arbalest*>(this)->getSkillTargetType(skillNumber);
}

TargetType Arbalest::getSkillTargetType(int skillNumber) const
{
    return skillNumber == 2 ? TargetType::Ally : TargetType::Enemy;
}

void Arbalest::sniperShot(Character& target)
{
    std::cout << "Arbalest lines up a Sniper Shot!\n";
    Character::attack(target);
}

void Arbalest::battlefieldBandage(Character& target)
{
    std::cout << "Arbalest applies a battlefield bandage to "
              << target.getName() << ".\n";
    target.heal(4);
}
