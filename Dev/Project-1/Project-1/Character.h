#pragma once

#include <string>

struct Stats
{
    int maxHP;
    int speed;
    int accuracy;
    int minDamage;
    int maxDamage;
    int critChance;
};
enum class TargetType
{
    Enemy,
    Ally
};
class Character
{
protected:
    std::string name;
    Stats stats;

    int currentHP;
    int stress;

    int position;

public:

    
    // Constructor
    Character(const std::string& name, const Stats& stats);

    // Destructor
    virtual ~Character();

 
    // Getters
    const std::string& getName() const;
    int getHP() const;
    int getMaxHP() const;
    int getSpeed() const;
    int getAccuracy() const;
    int getMinDamage() const;
    int getMaxDamage() const;
    int getCritChance() const;
    int getStress() const;
    int getPosition() const;

    // State checks
    bool isAlive() const;

    // Combat functions
    virtual void attack(Character& target);
    void takeDamage(int damage);
    void heal(int amount);
    void setPosition(int newPosition);

    virtual TargetType getSkillTargetType(int skillNumber) const;
    virtual void printSkills() const;
    virtual void useSkill(int skillNumber, Character& target);
    // Stress

    void addStress(int amount);
    void reduceStress(int amount);

    // Display
    virtual void printInfo() const;
};