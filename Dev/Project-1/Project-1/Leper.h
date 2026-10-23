#pragma once

#include "Character.h"

class Leper : public Character
{
public:
    Leper();

    void printSkills() const override;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber) const override;

private:
    void chop(Character& target);
    void solemnity(Character& target);
};
