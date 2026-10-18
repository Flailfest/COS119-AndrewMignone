#pragma once

#include "Character.h"

class Hellion : public Character
{
public:
    Hellion();

    void printSkills();
    void printSkills() const;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber);
    TargetType getSkillTargetType(int skillNumber) const;

private:
    void wickedGlaive(Character& target);
    void ironGlaive(Character& target);
    void printSkillList() const;
};
