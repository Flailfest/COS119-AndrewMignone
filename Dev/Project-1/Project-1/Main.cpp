#include "Character.h"
#include <iostream>


int main()
{
    Stats crusaderStats
    {
        33, // maxHP
        1,  // speed
        85, // accuracy
        6,  // minDamage
        10, // maxDamage
        5   // critChance
    };

    Character crusader(
        "Crusader",
        crusaderStats
    );

    Stats skeletonStats
    {
        20, // maxHP
        2,  // speed
        80, // accuracy
        4,  // minDamage
        7,  // maxDamage
        2   // critChance
    };

    Character skeleton(
        "Skeleton",
        skeletonStats
    );

    crusader.printInfo();
    skeleton.printInfo();

    crusader.attack(skeleton);

    skeleton.addStress(10);

    crusader.takeDamage(5);

    crusader.heal(3);

    return 0;
}