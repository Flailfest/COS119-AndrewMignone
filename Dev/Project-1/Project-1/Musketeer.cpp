#include "Musketeer.h"

#include <iostream>

Musketeer::Musketeer()
    : Character("Musketeer", { 24, 5, 90, 5, 10, 9 })
{
}

void Musketeer::printSkills()
{
    printSkillList();
}

void Musketeer::printSkills() const
{
    printSkillList();
}

void Musketeer::printSkillList() const
{
    std::cout << "1. Aimed Shot (long-range shot; targets any enemy slot)\n";
    std::cout << "2. Piercing Shot (long-range shot; targets enemy slots 1-3)\n";
}

void Musketeer::useSkill(int skillNumber, Character& target)
{
    if (skillNumber == 1)
    {
        aimedShot(target);
    }
    else if (skillNumber == 2)
    {
        piercingShot(target);
    }
}

TargetType Musketeer::getSkillTargetType(int skillNumber)
{
    return static_cast<const Musketeer*>(this)->getSkillTargetType(skillNumber);
}

TargetType Musketeer::getSkillTargetType(int /*skillNumber*/) const
{
    return TargetType::Enemy;
}

void Musketeer::aimedShot(Character& target)
{
    std::cout << "Musketeer steadies for an Aimed Shot!\n";
    Character::attack(target);
}

void Musketeer::piercingShot(Character& target)
{
    std::cout << "Musketeer fires a Piercing Shot!\n";
    Character::attack(target);
}
