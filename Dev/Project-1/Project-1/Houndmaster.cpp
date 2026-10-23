#include "Houndmaster.h"

#include <iostream>

Houndmaster::Houndmaster()
    : Character("Houndmaster", { 27, 6, 85, 4, 8, 8 })
{
}

void Houndmaster::printSkills() const
{
    std::cout << "1. Hound's Rush (strike an enemy from any rank)\n";
    std::cout << "2. Blackjack (close-range strike that stuns a surviving target)\n";
}

void Houndmaster::useSkill(int skillNumber, Character& target)
{
    if (skillNumber == 1)
    {
        houndsRush(target);
    }
    else if (skillNumber == 2)
    {
        blackjack(target);
    }
}

TargetType Houndmaster::getSkillTargetType(int /*skillNumber*/) const
{
    return TargetType::Enemy;
}

void Houndmaster::houndsRush(Character& target)
{
    std::cout << "The Houndmaster sends his hound after "
              << target.getName() << ".\n";
    Character::attack(target);
}

void Houndmaster::blackjack(Character& target)
{
    std::cout << "The Houndmaster strikes " << target.getName()
              << " with his blackjack.\n";
    Character::attack(target);
}
