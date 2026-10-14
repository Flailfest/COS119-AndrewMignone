
#include "PlagueDoctor.h"

#include <iostream>


// ==================================================
// CONSTRUCTOR
// ==================================================
//
// Defines the Plague Doctor's starting statistics.
//
// These values are passed to the base Character
// constructor, which initializes the character's
// name, statistics, HP, stress, and other properties.
//
// Character positions are managed by Formation.
//
PlagueDoctor::PlagueDoctor()
    : Character(
        "Plague Doctor",
        {
            22, // Maximum HP
            6,  // Speed
            85, // Accuracy
            3,  // Minimum damage
            6,  // Maximum damage
            7   // Critical hit chance
        })
{
}


// ==================================================
// SKILL 1: PLAGUE GRENADE
// ==================================================
//
// Throws a Plague Grenade at the selected enemy.
//
// Battle.cpp is responsible for checking whether
// the Plague Doctor can use this skill from their
// current position and whether the selected enemy
// is within the skill's target range.
//
// Expected restrictions:
// - Plague Doctor must be in position 1 or 2.
// - Enemy must be in position 3 or 4.
//
// Character::attack(target) explicitly invokes the
// base Character attack implementation.
//
// This is important because we are removing the
// Plague Doctor's basic-attack override. Explicitly
// calling the base implementation also prevents
// future derived attack restrictions from interfering
// with this skill.
//
void PlagueDoctor::plagueGrenade(Character& target)
{
    std::cout
        << getName()
        << " throws a Plague Grenade at "
        << target.getName()
        << "!\n";

    // Apply the existing damage behavior.
    Character::attack(target);
}


// ==================================================
// SKILL 2: HEAL ALLY
// ==================================================
//
// Battlefield Medicine restores 4 HP to the
// selected friendly character.
//
// Battle.cpp handles ally selection and validates
// whether the skill is allowed to target that ally.
//
// Character::heal() ensures that the target's HP
// cannot exceed their maximum HP.
//
void PlagueDoctor::healAlly(Character& target)
{
    std::cout
        << getName()
        << " uses Battlefield Medicine on "
        << target.getName()
        << "!\n";

    target.heal(4);

    std::cout
        << target.getName()
        << " recovers 4 HP.\n";
}


// ==================================================
// DISPLAY SKILLS
// ==================================================
//
// Prints the Plague Doctor's available skills.
//
// Battle.cpp calls this function when displaying
// the character's skill-selection menu.
//
// The displayed skill numbers must match the cases
// in useSkill() and the skill rules in Battle.cpp.
//
void PlagueDoctor::printSkills() const
{
    std::cout << "1. Plague Grenade\n";
    std::cout << "2. Heal Ally\n";
}


// ==================================================
// EXECUTE A SKILL
// ==================================================
//
// Receives the player's chosen skill number and
// the character selected as its target.
//
// This function dispatches execution to the
// appropriate skill method.
//
// It does not perform position or range checks.
// Those validations happen in Battle.cpp before
// useSkill() is called.
//
void PlagueDoctor::useSkill(
    int skillNumber,
    Character& target)
{
    switch (skillNumber)
    {
    case 1:
        // Damage the selected enemy.
        plagueGrenade(target);
        break;

    case 2:
        // Restore HP to the selected ally.
        healAlly(target);
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
//     The player selects an enemy target.
//
// TargetType::Ally:
//     The player selects a friendly target.
//
// This function only identifies the target category.
// It does not select a character or execute the skill.
//
TargetType PlagueDoctor::getSkillTargetType(
    int skillNumber) const
{
    switch (skillNumber)
    {
    case 1:
        // Plague Grenade targets an enemy.
        return TargetType::Enemy;

    case 2:
        // Battlefield Medicine targets an ally.
        return TargetType::Ally;

    default:
        // Preserve the existing default behavior.
        return TargetType::Enemy;
    }
}
