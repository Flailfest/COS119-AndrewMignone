#include "BattleRules.h"
#include "Character.h"

#include <string>

namespace BattleRules
{
    bool getAttackRules(const Character& character, PositionRules& rules)
    {
        const std::string name = character.getName();

        if (name == "Crusader")
        {
            rules = { 1, 2, 1, 2 };
            return true;
        }

        if (name == "Highwayman")
        {
            rules = { 1, 3, 2, 4 };
            return true;
        }

        if (name == "Plague Doctor")
        {
            rules = { 1, 3, 1, 2 };
            return true;
        }

        if (name == "Vestal")
        {
            rules = { 1, 4, 1, 2 };
            return true;
        }

        return false;
    }

    bool getSkillRules(const Character& character, int skillNumber, PositionRules& rules)
    {
        const std::string name = character.getName();
        rules = { 1, 4, 1, 4 };

        if (name == "Crusader")
        {
            if (skillNumber == 1)
            {
                rules = { 1, 2, 3, 4 };
                return true;
            }
            if (skillNumber == 2)
            {
                return true;
            }
        }
        else if (name == "Highwayman")
        {
            if (skillNumber == 1)
            {
                rules = { 1, 3, 2, 4 };
                return true;
            }
            if (skillNumber == 2)
            {
                rules = { 1, 2, 1, 2 };
                return true;
            }
        }
        else if (name == "Plague Doctor")
        {
            if (skillNumber == 1)
            {
                rules = { 1, 2, 3, 4 };
                return true;
            }
            if (skillNumber == 2)
            {
                rules = { 1, 4, 1, 4 };
                return true;
            }
        }
        else if (name == "Vestal")
        {
            if (skillNumber == 1)
            {
                rules = { 1, 4, 1, 2 };
                return true;
            }
            if (skillNumber == 2)
            {
                rules = { 1, 4, 1, 4 };
                return true;
            }
        }

        return false;
    }

    bool isPositionInRange(int position, int minimumPosition, int maximumPosition)
    {
        return position >= minimumPosition && position <= maximumPosition;
    }
}
