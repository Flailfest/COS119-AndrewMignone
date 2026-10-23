#pragma once

#include "Character.h"

class GraveRobber : public Character
{
public:
    GraveRobber();

    void printSkills() const override;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber) const override;

private:
    void pickToTheFace(Character& target);
    void thrownDagger(Character& target);
};
