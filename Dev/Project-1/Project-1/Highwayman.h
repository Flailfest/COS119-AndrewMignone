#pragma once

#include "Character.h"

class Highwayman : public Character
{
public:
    Highwayman();

    void pistolShot(Character& target);
    void meleeAttack(Character& target);

    void printSkills() const override;
    void useSkill(int skillNumber, Character& target) override;
    void attack(Character& target) override;
};