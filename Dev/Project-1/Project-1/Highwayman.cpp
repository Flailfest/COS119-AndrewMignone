
#include "Highwayman.h"

#include <iostream>


// ==================================================
// CONSTRUCTOR
// ==================================================
//
// Defines the Highwayman's starting statistics.
//
// These values are passed to the base Character
// constructor, which initializes the character's
// name, stats, HP, stress, and other properties.
//
// Formation manages the Highwayman's position.
//
Highwayman::Highwayman()
    : Character(
        "Highwayman",
        {
            26, // Maximum HP
            7,  // Speed
            90, // Accuracy
            4,  // Minimum damage
            8,  // Maximum damage
            10  // Critical hit chance
        })
{
}


// ==================================================
// SKILL 1: PISTOL SHOT
// ==================================================
//
// Fires a pistol at the selected enemy.
//
// Battle.cpp validates the positions before this
// method is called.
//
// Expected restrictions:
// - Highwayman must be in position 1, 2, or 3.
// - Enemy must be in position 2, 3, or 4.
//
// Character::attack(target) explicitly calls the
// base attack implementation.
//
// This prevents an overridden basic attack from
// applying its own range restrictions to Pistol Shot.
//
void Highwayman::pistolShot(Character& target)
{
    std::cout
        << getName()
        << " uses Pistol Shot on "
        << target.getName()
        << "!\n";

    Character::attack(target);
}


// ==================================================
// SKILL 2: MELEE ATTACK
// ==================================================
//
// Performs a melee attack against the selected enemy.
//
// Battle.cpp validates the positions before this
// method is called.
//
// Expected restrictions:
// - Highwayman must be in position 1 or 2.
// - Enemy must be in position 1 or 2.
//
// No position checks are performed here because
// Formation and Battle handle movement and validation.
//
// The damage is resolved through Character::attack(),
// keeping the damage calculation consistent with
// the other characters.
//
void Highwayman::meleeAttack(Character& target)
{
    std::cout
        << getName()
        << " uses Melee Attack on "
        << target.getName()
        << "!\n";

    Character::attack(target);
}


// ==================================================
// DISPLAY SKILLS
// ==================================================
//
// Displays the Highwayman's available skills.
//
// Battle.cpp calls this method when the player
// opens the Highwayman's skill menu.
//
// Keep these numbers consistent with useSkill()
// and the rules defined in Battle.cpp.
//
void Highwayman::printSkills() const
{
    std::cout << "1. Pistol Shot\n";
    std::cout << "2. Melee Attack\n";
}


// ==================================================
// EXECUTE A SKILL
// ==================================================
//
// Receives the chosen skill number and target.
//
// This method dispatches the request to the
// appropriate skill implementation.
//
// It does not check formation positions or target
// ranges. Battle.cpp handles those checks before
// invoking this method.
//
void Highwayman::useSkill(
    int skillNumber,
    Character& target)
{
    switch (skillNumber)
    {
    case 1:
        // Use the ranged Pistol Shot skill.
        pistolShot(target);
        break;

    case 2:
        // Use the close-range Melee Attack skill.
        meleeAttack(target);
        break;

    default:
        // Safety check for an unsupported skill number.
        std::cout << "Invalid skill.\n";
        break;
    }
}


// ==================================================
// IDENTIFY SKILL TARGET TYPE
// ==================================================
//
// Tells Battle.cpp which target-selection menu
// should be displayed for each skill.
//
// TargetType::Enemy:
//     The player chooses an enemy.
//
// TargetType::Ally:
//     The player chooses a friendly character.
//
// Both Highwayman skills target enemies.
//
// This method identifies the target category only;
// it does not validate positions or execute attacks.
//
TargetType Highwayman::getSkillTargetType(
    int skillNumber) const
{
    switch (skillNumber)
    {
    case 1:
        // Pistol Shot targets an enemy.
        return TargetType::Enemy;

    case 2:
        // Melee Attack targets an enemy.
        return TargetType::Enemy;

    default:
        // Preserve the existing default behavior.
        return TargetType::Enemy;
    }
}
