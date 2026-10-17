#pragma once

#include "Character.h"

class ManAtArms : public Character
{
public:
    ManAtArms();

    void printSkills();
    void printSkills() const;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber);
    TargetType getSkillTargetType(int skillNumber) const;

private:
    void crush(Character& target);
    void shieldBash(Character& target);
    void printSkillList() const;
};
