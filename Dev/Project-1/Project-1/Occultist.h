#pragma once

#include "Character.h"

class Occultist : public Character
{
public:
    Occultist();

    void printSkills() const override;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber) const override;

private:
    void sacrificialStab(Character& target);
    void wyrdReconstruction(Character& target);
};
