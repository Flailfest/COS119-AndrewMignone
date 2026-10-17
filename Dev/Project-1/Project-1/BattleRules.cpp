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
        if (name == "Arbalest")
        {
            rules = { 2, 3, 1, 4 };
            return true;
        }
        if (name == "Musketeer")
        {
            rules = { 3, 4, 1, 4 };
            return true;
        }
        if (name == "Man-at-Arms")
        {
            rules = { 1, 2, 1, 2 };
            return true;
        }
        if (name == "Hellion")
        {
            rules = { 1, 2, 1, 2 };
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
            if (skillNumber == 2) // Grapeshot
            {
                // Usable from Highwayman positions 1-2; hits enemy positions 1-3.
                rules = { 1, 2, 1, 3 };
                return true;
            }
        }
        else if (name == "Plague Doctor")
        {
            if (skillNumber == 1) // Plague Grenade
            {
                // The Plague Doctor must be in the back two slots (3-4).
                // Plague Grenade targets the enemy back two slots (3-4).
                rules = { 3, 4, 3, 4 };
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
        else if (name == "Arbalest")
        {
            if (skillNumber == 1) // Sniper Shot
            {
                rules = { 2, 3, 1, 4 };
                return true;
            }
            if (skillNumber == 2) // Battlefield Bandage
            {
                rules = { 1, 4, 1, 4 };
                return true;
            }
        }
        else if (name == "Musketeer")
        {
            if (skillNumber == 1) // Aimed Shot
            {
                rules = { 3, 4, 1, 4 };
                return true;
            }
            if (skillNumber == 2) // Piercing Shot
            {
                rules = { 3, 4, 1, 3 };
                return true;
            }
        }
        else if (name == "Man-at-Arms")
        {
            if (skillNumber == 1) // Crush
            {
                rules = { 1, 2, 1, 2 };
                return true;
            }
            if (skillNumber == 2) // Shield Bash
            {
                rules = { 1, 2, 1, 2 };
                return true;
            }
        }
        else if (name == "Hellion")
        {
            if (skillNumber == 1) // Wicked Glaive
            {
                rules = { 1, 2, 1, 3 };
                return true;
            }
            if (skillNumber == 2) // Iron Glaive, strongest from position 2
            {
                rules = { 2, 2, 1, 3 };
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
