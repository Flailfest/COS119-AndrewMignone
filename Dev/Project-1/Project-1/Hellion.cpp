#include "Hellion.h"

#include <iostream>

Hellion::Hellion()
    : Character("Hellion", { 32, 5, 85, 5, 10, 7 })
{
}

void Hellion::printSkills()
{
    printSkillList();
}

void Hellion::printSkills() const
{
    printSkillList();
}

void Hellion::printSkillList() const
{
    std::cout << "1. Wicked Glaive (can reach enemy slots 1-3)\n";
    std::cout << "2. Iron Glaive (strongest from position 2; reaches slots 1-3)\n";
}

void Hellion::useSkill(int skillNumber, Character& target)
{
    if (skillNumber == 1)
    {
        wickedGlaive(target);
    }
    else if (skillNumber == 2)
    {
        ironGlaive(target);
    }
}

TargetType Hellion::getSkillTargetType(int skillNumber)
{
    return static_cast<const Hellion*>(this)->getSkillTargetType(skillNumber);
}

TargetType Hellion::getSkillTargetType(int /*skillNumber*/) const
{
    return TargetType::Enemy;
}

void Hellion::wickedGlaive(Character& target)
{
    std::cout << "Hellion sweeps her glaive toward "
              << target.getName() << ".\n";
    Character::attack(target);
}

void Hellion::ironGlaive(Character& target)
{
    std::cout << "Hellion drives her glaive into "
              << target.getName() << ".\n";
    Character::attack(target);
}
