
#include "Vestal.h"

#include <iostream>


// ==================================================
// CONSTRUCTOR
// ==================================================
//
// Defines the Vestal's starting statistics.
//
// These values are passed to the base Character
// constructor, which initializes the character's
// name, statistics, HP, stress, and other properties.
//
// Character positions are managed through Formation.
// We do not assign or validate positions here.
//
Vestal::Vestal()
    : Character(
        "Vestal",
        {
            24, // Maximum HP
            4,  // Speed
            85, // Accuracy
            3,  // Minimum damage
            6,  // Maximum damage
            5   // Critical hit chance
        })
{
}


// ==================================================
// SKILL 1: SMITE
// ==================================================
//
// Deals damage to the selected enemy.
//
// Battle.cpp handles the position and target checks
// before this skill is executed.
//
// Expected restrictions:
// - Vestal can use Smite from positions 1-4.
// - Smite can target enemies in positions 1-2.
//
// Character::attack(target) explicitly calls the
// base Character attack implementation.
//
// We use this instead of attack(target) so we do not
// accidentally invoke an overridden basic attack
// with its own range restrictions.
//
void Vestal::smite(Character& target)
{
    std::cout
        << getName()
        << " uses Smite on "
        << target.getName()
        << "!\n";

    // Execute the existing base damage behavior.
    Character::attack(target);
}


// ==================================================
// SKILL 2: HEAL ALLY
// ==================================================
//
// Divine Heal restores up to 8 HP to the selected
// friendly character.
//
// Battle.cpp handles ally selection and target
// validation before calling this skill.
//
// Character::heal() prevents HP from exceeding the
// target's maximum HP.
//
void Vestal::healAlly(Character& target)
{
    std::cout
        << getName()
        << " uses Divine Heal on "
        << target.getName()
        << "!\n";

    target.heal(8);

    std::cout
        << target.getName()
        << " receives up to 8 HP of healing.\n";
}


// ==================================================
// DISPLAY SKILLS
// ==================================================
//
// Displays the Vestal's available skills.
//
// Battle.cpp calls this when the player opens
// the Vestal's skill menu.
//
// Keep these skill numbers consistent with the
// switch cases below and the rules in Battle.cpp.
//
void Vestal::printSkills() const
{
    std::cout << "1. Smite\n";
    std::cout << "2. Heal Ally\n";
}


// ==================================================
// EXECUTE A SKILL
// ==================================================
//
// Receives the selected skill number and target.
//
// This function dispatches execution to the matching
// skill method.
//
// It does not check positions or target ranges.
// Battle.cpp is responsible for those validations.
//
void Vestal::useSkill(
    int skillNumber,
    Character& target)
{
    switch (skillNumber)
    {
    case 1:
        // Damage the selected enemy.
        smite(target);
        break;

    case 2:
        // Heal the selected ally.
        healAlly(target);
        break;

    default:
        // Safety check for invalid skill numbers.
        std::cout << "Invalid skill.\n";
        break;
    }
}


// ==================================================
// IDENTIFY SKILL TARGET TYPE
// ==================================================
//
// Tells Battle.cpp which target-selection menu to use.
//
// TargetType::Enemy:
//     The player selects an enemy.
//
// TargetType::Ally:
//     The player selects a friendly character.
//
// This function only identifies the target category.
// It does not perform the skill or select the target.
//
TargetType Vestal::getSkillTargetType(
    int skillNumber) const
{
    switch (skillNumber)
    {
    case 1:
        // Smite targets an enemy.
        return TargetType::Enemy;

    case 2:
        // Heal Ally targets a friendly character.
        return TargetType::Ally;

    default:
        // Preserve the existing default behavior.
        return TargetType::Enemy;
    }
}
