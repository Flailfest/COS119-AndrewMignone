#pragma once

#include "Character.h"

class Musketeer : public Character
{
public:
    Musketeer();

    // Provide const and non-const forms to match the existing Character interface.
    void printSkills();
    void printSkills() const;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber);
    TargetType getSkillTargetType(int skillNumber) const;

private:
    void aimedShot(Character& target);
    void piercingShot(Character& target);
    void printSkillList() const;
};
