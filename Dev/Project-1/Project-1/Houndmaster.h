#pragma once

#include "Character.h"

class Houndmaster : public Character
{
public:
    Houndmaster();

    void printSkills() const override;
    void useSkill(int skillNumber, Character& target) override;
    TargetType getSkillTargetType(int skillNumber) const override;

private:
    void houndsRush(Character& target);
    void blackjack(Character& target);
};
