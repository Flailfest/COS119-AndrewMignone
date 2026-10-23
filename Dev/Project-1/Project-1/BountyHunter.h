#pragma once

#include "Character.h"

class BountyHunter : public Character
{
public:
    BountyHunter();

    void printSkills() const override;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber) const override;

private:
    void collectBounty(Character& target);
    void finishHim(Character& target);
};
