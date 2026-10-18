#include "ManAtArms.h"

#include <iostream>

ManAtArms::ManAtArms()
    : Character("Man-at-Arms", { 40, 2, 80, 2, 5, 3 })
{
}

void ManAtArms::printSkills()
{
    printSkillList();
}

void ManAtArms::printSkills() const
{
    printSkillList();
}

void ManAtArms::printSkillList() const
{
    std::cout << "1. Crush (melee damage to enemy slots 1-2)\n";
    std::cout << "2. Shield Bash (damage and stun an enemy in slots 1-2)\n";
}

void ManAtArms::useSkill(int skillNumber, Character& target)
{
    if (skillNumber == 1)
    {
        crush(target);
    }
    else if (skillNumber == 2)
    {
        shieldBash(target);
    }
}

TargetType ManAtArms::getSkillTargetType(int skillNumber)
{
    return static_cast<const ManAtArms*>(this)->getSkillTargetType(skillNumber);
}

TargetType ManAtArms::getSkillTargetType(int /*skillNumber*/) const
{
    return TargetType::Enemy;
}

void ManAtArms::crush(Character& target)
{
    std::cout << "Man-at-Arms brings down his weapon on "
              << target.getName() << ".\n";
    Character::attack(target);
}

void ManAtArms::shieldBash(Character& target)
{
    std::cout << "Man-at-Arms drives his shield into "
              << target.getName() << ".\n";
    Character::attack(target);
}
