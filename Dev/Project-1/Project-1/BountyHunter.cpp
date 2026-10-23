#include "BountyHunter.h"

#include <iostream>

BountyHunter::BountyHunter()
    : Character("Bounty Hunter", { 30, 5, 85, 5, 10, 8 })
{
}

void BountyHunter::printSkills() const
{
    std::cout << "1. Collect Bounty (heavy strike against a front-rank enemy)\n";
    std::cout << "2. Finish Him (follow-up strike against a front or middle enemy)\n";
}

void BountyHunter::useSkill(int skillNumber, Character& target)
{
    if (skillNumber == 1)
    {
        collectBounty(target);
    }
    else if (skillNumber == 2)
    {
        finishHim(target);
    }
}

TargetType BountyHunter::getSkillTargetType(int /*skillNumber*/) const
{
    return TargetType::Enemy;
}

void BountyHunter::collectBounty(Character& target)
{
    std::cout << "The Bounty Hunter marks his quarry with a brutal strike: "
              << target.getName() << ".\n";
    Character::attack(target);
}

void BountyHunter::finishHim(Character& target)
{
    std::cout << "The Bounty Hunter presses the attack against "
              << target.getName() << ".\n";
    Character::attack(target);
}
