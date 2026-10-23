#include "Occultist.h"

#include <iostream>
#include <random>

Occultist::Occultist()
    : Character("Occultist", { 23, 7, 80, 4, 8, 10 })
{
}

void Occultist::printSkills() const
{
    std::cout << "1. Sacrificial Stab (melee attack against enemy front ranks)\n";
    std::cout << "2. Wyrd Reconstruction (unpredictably heal an ally for 2-8 HP)\n";
}

void Occultist::useSkill(int skillNumber, Character& target)
{
    if (skillNumber == 1)
    {
        sacrificialStab(target);
    }
    else if (skillNumber == 2)
    {
        wyrdReconstruction(target);
    }
}

TargetType Occultist::getSkillTargetType(int skillNumber) const
{
    return skillNumber == 2 ? TargetType::Ally : TargetType::Enemy;
}

void Occultist::sacrificialStab(Character& target)
{
    std::cout << "The Occultist draws power from the void and strikes "
              << target.getName() << ".\n";
    Character::attack(target);
}

void Occultist::wyrdReconstruction(Character& target)
{
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> healingDistribution(2, 8);
    const int healing = healingDistribution(generator);

    std::cout << "The Occultist calls on forbidden powers to heal "
              << target.getName() << ".\n";
    target.heal(healing);
}
