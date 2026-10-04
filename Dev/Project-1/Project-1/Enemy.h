#pragma once

#include "Character.h"

class Enemy : public Character
{
protected:
    int goldReward;
    int experienceReward;

public:
    Enemy(
        const std::string& name,
        const Stats& stats,
        int goldReward,
        int experienceReward
    );

    virtual ~Enemy();

    // Enemy-specific getters
    int getGoldReward() const;
    int getExperienceReward() const;

    // Enemy behavior
    virtual void performTurn(Character& target);

    // Display
    void printInfo() const override;
};

