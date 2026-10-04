#pragma once

#include "Character.h"

class Vestal : public Character
{
public:
    Vestal();

    void healAlly(Character& target);
    void smite(Character& target);

    void printSkills() const override;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber) const override;
    void attack(Character& target) override;
};