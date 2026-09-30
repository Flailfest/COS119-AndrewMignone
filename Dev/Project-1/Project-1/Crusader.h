#pragma once

#include "Character.h"

class Crusader : public Character
{
public:
    Crusader();

    void holyStrike(Character& target);
    void healAlly(Character& target);
    void printSkills() const override;
    void useSkill(int skillNumber, Character& target) override;
};
