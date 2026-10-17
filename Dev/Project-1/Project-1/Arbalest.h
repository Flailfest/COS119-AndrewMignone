#pragma once

#include "Character.h"

class Arbalest : public Character
{
public:
    Arbalest();

    // Provide const and non-const forms to match the existing Character interface.
    void printSkills();
    void printSkills() const;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber);
    TargetType getSkillTargetType(int skillNumber) const;

private:
    void sniperShot(Character& target);
    void battlefieldBandage(Character& target);
    void printSkillList() const;
};
