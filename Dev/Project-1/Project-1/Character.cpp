#include "Character.h"

#include <iostream>
#include <algorithm>
#include <random>

// --------------------------------------------------
// Constructor
// --------------------------------------------------

Character::Character(
    const std::string& name,
    const Stats& stats
)
    : name(name),
    stats(stats),
    currentHP(stats.maxHP),
    stress(0),
    position(0)
{
}

// --------------------------------------------------
// Destructor
// --------------------------------------------------

Character::~Character()
{
}

// --------------------------------------------------
// Getters
// --------------------------------------------------

const std::string& Character::getName() const
{
    return name;
}

int Character::getHP() const
{
    return currentHP;
}

int Character::getMaxHP() const
{
    return stats.maxHP;
}

int Character::getSpeed() const
{
    return stats.speed;
}

int Character::getAccuracy() const
{
    return stats.accuracy;
}

int Character::getMinDamage() const
{
    return stats.minDamage;
}

int Character::getMaxDamage() const
{
    return stats.maxDamage;
}

int Character::getCritChance() const
{
    return stats.critChance;
}

int Character::getStress() const
{
    return stress;
}

int Character::getPosition() const
{
    return position;
}

// --------------------------------------------------
// State checks
// --------------------------------------------------

bool Character::isAlive() const
{
    return currentHP > 0;
}

// --------------------------------------------------
// Combat
// --------------------------------------------------

void Character::attack(Character& target)
{
    // Create a random number generator
    static std::random_device rd;
    static std::mt19937 generator(rd());

    // Create a distribution between min and max damage
    std::uniform_int_distribution<int> distribution(
        stats.minDamage,
        stats.maxDamage
    );

    // Generate the damage
    int damage = distribution(generator);

    std::cout
        << name
        << " attacks "
        << target.getName()
        << " for "
        << damage
        << " damage!\n";

    target.takeDamage(damage);
}

void Character::takeDamage(int damage)
{
    // Don't allow negative damage
    damage = std::max(0, damage);

    currentHP -= damage;

    // Don't allow HP to go below zero
    currentHP = std::max(0, currentHP);

    std::cout
        << name
        << " has "
        << currentHP
        << "/"
        << stats.maxHP
        << " HP remaining.\n";

    if (!isAlive())
    {
        std::cout
            << name
            << " has been defeated!\n";
    }
}

void Character::heal(int amount)
{
    // Don't allow negative healing
    amount = std::max(0, amount);

    currentHP += amount;

    // Don't allow HP to exceed maximum
    currentHP = std::min(
        currentHP,
        stats.maxHP
    );

    std::cout
        << name
        << " heals for "
        << amount
        << " HP.\n";
}

// --------------------------------------------------
// Stress
// --------------------------------------------------

void Character::addStress(int amount)
{
    amount = std::max(0, amount);

    stress += amount;

    // Darkest Dungeon-style stress cap
    stress = std::min(200, stress);

    std::cout
        << name
        << " gains "
        << amount
        << " stress.\n";

    std::cout
        << "Stress: "
        << stress
        << "/200\n";
}

void Character::reduceStress(int amount)
{
    amount = std::max(0, amount);

    stress -= amount;

    stress = std::max(0, stress);

    std::cout
        << name
        << " loses "
        << amount
        << " stress.\n";

    std::cout
        << "Stress: "
        << stress
        << "/200\n";
}


void Character::setPosition(int newPosition)
{
    if (newPosition < 0 || newPosition > 4)
    {
        std::cout << "Invalid position.\n";
        return;
    }

    position = newPosition;
}


// --------------------------------------------------
// Display
// --------------------------------------------------

void Character::printInfo() const
{
    std::cout << "\n";
    std::cout << "============================\n";
    std::cout << name << "\n";
    std::cout << "============================\n";

    std::cout
        << "HP: "
        << currentHP
        << "/"
        << stats.maxHP
        << "\n";

    std::cout
        << "Stress: "
        << stress
        << "/200\n";

    std::cout
        << "Speed: "
        << stats.speed
        << "\n";

    std::cout
        << "Accuracy: "
        << stats.accuracy
        << "\n";

    std::cout
        << "Damage: "
        << stats.minDamage
        << "-"
        << stats.maxDamage
        << "\n";

    std::cout
        << "Crit Chance: "
        << stats.critChance
        << "%\n";

    std::cout << "============================\n";
}