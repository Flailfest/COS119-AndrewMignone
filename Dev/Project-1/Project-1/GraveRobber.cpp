#include "GraveRobber.h"

#include <iostream>

GraveRobber::GraveRobber()
    : Character("Grave Robber", { 24, 8, 90, 4, 8, 10 })
{
}

void GraveRobber::printSkills() const
{
    std::cout << "1. Pick to the Face (melee attack against enemy front ranks)\n";
    std::cout << "2. Thrown Dagger (ranged attack against enemy back ranks)\n";
}

void GraveRobber::useSkill(int skillNumber, Character& target)
{
    if (skillNumber == 1)
    {
        pickToTheFace(target);
    }
    else if (skillNumber == 2)
    {
        thrownDagger(target);
    }
}

TargetType GraveRobber::getSkillTargetType(int /*skillNumber*/) const
{
    return TargetType::Enemy;
}

void GraveRobber::pickToTheFace(Character& target)
{
    std::cout << "The Grave Robber strikes at "
              << target.getName() << " with her pickaxe.\n";
    Character::attack(target);
}

void GraveRobber::thrownDagger(Character& target)
{
    std::cout << "The Grave Robber throws a dagger at "
              << target.getName() << ".\n";
    Character::attack(target);
}
