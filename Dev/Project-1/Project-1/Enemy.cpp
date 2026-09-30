#include "Enemy.h"

#include <iostream>

// --------------------------------------------------
// Constructor
// --------------------------------------------------

Enemy::Enemy(
    const std::string& name,
    const Stats& stats,
    int goldReward,
    int experienceReward
)
    : Character(name, stats),
    goldReward(goldReward),
    experienceReward(experienceReward)
{
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------

Enemy::~Enemy()
{
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

int Enemy::getGoldReward() const
{
    return goldReward;
}

int Enemy::getExperienceReward() const
{
    return experienceReward;
}

// --------------------------------------------------
// Enemy Turn
// --------------------------------------------------

void Enemy::performTurn(Character& target)
{
    // Basic enemy behavior for now:
    // attack the target.

    attack(target);
}

// --------------------------------------------------
// Display
// --------------------------------------------------

void Enemy::printInfo() const
{
    Character::printInfo();

    std::cout
        << "Gold Reward: "
        << goldReward
        << "\n";

    std::cout
        << "XP Reward: "
        << experienceReward
        << "\n";
}