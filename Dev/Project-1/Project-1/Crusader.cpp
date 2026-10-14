
#include "Crusader.h"

#include <iostream>

// ==================================================
// CONSTRUCTOR
// ==================================================
//
// Defines the Crusader's starting statistics.
//
// These values are passed to the base Character
// constructor, which initializes the character's
// name, stats, HP, stress, and other properties.
//
// Position is managed through the Formation system.
// We do not assign a position here.
//
Crusader::Crusader()
    : Character(
        "Crusader",
        {
            33, // Maximum HP
            3,  // Speed
            85, // Accuracy
            5,  // Minimum damage
            9,  // Maximum damage
            5   // Critical hit chance
        })
{
}


// ==================================================
// SKILL 1: HOLY STRIKE
// ==================================================
//
// Deals damage to the selected enemy.
//
// IMPORTANT:
// Battle.cpp is responsible for checking whether
// the Crusader can use this skill from their current
// position and whether the selected enemy is in range.
//
// The expected restrictions are:
// - Crusader must be in position 1 or 2.
// - Enemy must be in position 3 or 4.
//
// Character::attack(target) explicitly calls the
// base Character attack implementation.
//
// This avoids accidentally invoking a derived
// basic-attack override with different range rules.
//
void Crusader::holyStrike(Character& target)
{
    Character::attack(target);
}


// ==================================================
// SKILL 2: HEAL ALLY
// ==================================================
//
// Restores 5 HP to the selected ally.
//
// Battle.cpp handles target selection and validates
// the positions allowed by the skill.
//
// Character::heal() handles the healing itself,
// including ensuring that HP cannot exceed max HP.
//
void Crusader::healAlly(Character& target)
{
    target.heal(5);

    std::cout
        << target.getName()
        << " is healed for 5 HP by the Crusader.\n";
}


// ==================================================
// DISPLAY SKILLS
// ==================================================
//
// Prints the Crusader's available skills.
//
// Battle.cpp calls this function when the player
// opens the Crusader's skill menu.
//
// Keep the displayed numbers consistent with the
// switch cases in useSkill() and the target rules
// defined in Battle.cpp.
//
void Crusader::printSkills() const
{
    std::cout << "1. Holy Strike\n";
    std::cout << "2. Heal Ally\n";
}


// ==================================================
// EXECUTE A SKILL
// ==================================================
//
// Receives the skill number chosen by the player
// and the character selected as the target.
//
// This function dispatches execution to the
// appropriate skill method.
//
// It does not perform position or range validation.
// Those checks happen in Battle.cpp before this
// function is called.
//
void Crusader::useSkill(
    int skillNumber,
    Character& target)
{
    switch (skillNumber)
    {
    case 1:
        // Damage the selected enemy.
        holyStrike(target);
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
// Tells Battle.cpp what kind of character the skill
// is allowed to target.
//
// TargetType::Enemy:
//     Battle.cpp opens the enemy target-selection menu.
//
// TargetType::Ally:
//     Battle.cpp opens the ally target-selection menu.
//
// This function does not select a target or perform
// the skill. It only identifies the target category.
//
TargetType Crusader::getSkillTargetType(
    int skillNumber) const
{
    switch (skillNumber)
    {
    case 1:
        // Holy Strike targets an enemy.
        return TargetType::Enemy;

    case 2:
        // Heal Ally targets a friendly character.
        return TargetType::Ally;

    default:
        // Preserve the existing default behavior.
        return TargetType::Enemy;
    }
}
