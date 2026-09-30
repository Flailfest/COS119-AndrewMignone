#pragma once

#include "Character.h"

class PlagueDoctor : public Character
{
public:
    PlagueDoctor();

    void plagueGrenade(Character& target);
    void healAlly(Character& target);

    void printSkills() const override;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber) const override;
    void attack(Character& target) override;
};

