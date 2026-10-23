#include "Leper.h"

#include <iostream>

Leper::Leper()
    : Character("Leper", { 38, 2, 75, 8, 13, 5 })
{
}

void Leper::printSkills() const
{
    std::cout << "1. Chop (powerful melee attack against the front ranks)\n";
    std::cout << "2. Solemnity (heal self for 6 HP and reduce stress by 5)\n";
}

void Leper::useSkill(int skillNumber, Character& target)
{
    if (skillNumber == 1)
    {
        chop(target);
    }
    else if (skillNumber == 2)
    {
        solemnity(target);
    }
}

TargetType Leper::getSkillTargetType(int skillNumber) const
{
    return skillNumber == 2 ? TargetType::Self : TargetType::Enemy;
}

void Leper::chop(Character& target)
{
    std::cout << "The Leper brings his massive blade down on "
              << target.getName() << ".\n";
    Character::attack(target);
}

void Leper::solemnity(Character& target)
{
    std::cout << "The Leper steels himself and regains his composure.\n";
    target.heal(6);
    target.reduceStress(5);
}
