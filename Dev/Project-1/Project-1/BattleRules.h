#pragma once

class Character;

namespace BattleRules
{
    struct PositionRules
    {
        int userMin;
        int userMax;
        int targetMin;
        int targetMax;
    };

    bool getAttackRules(const Character& character, PositionRules& rules);
    bool getSkillRules(const Character& character, int skillNumber, PositionRules& rules);
    bool isPositionInRange(int position, int minimumPosition, int maximumPosition);
}
