#include "Vestal.h"
#include "Crusader.h"
#include "Highwayman.h"
#include "PlagueDoctor.h"
#include "Party.h"
#include "Battle.h"
#include "Enemy.h"

#include <iostream>

int main()
{
    Crusader crusader;
    Highwayman highwayman;
    PlagueDoctor plagueDoctor;
    Vestal vestal;

    Party playerParty;

    playerParty.addMember(crusader, 1);
    playerParty.addMember(highwayman, 2);
    playerParty.addMember(plagueDoctor, 3);
    playerParty.addMember(vestal, 4);

    // TEST CRUSADER SKILLS
    Enemy testEnemy(
        "Test Enemy",
        {
            20, // max HP
            3,  // speed
            80, // accuracy
            3,  // min damage
            6,  // max damage
            5   // crit chance
        },
        10, // gold
        20  // experience
    );

    std::cout << "\n";
    std::cout << "===== CRUSADER SKILLS =====\n";

    crusader.printSkills();

    std::cout << "\n";
    std::cout << "===== USING HOLY STRIKE =====\n";

    crusader.useSkill(1, testEnemy);

    std::cout << "\n";
    std::cout << "===== TEST ENEMY HP =====\n";

    std::cout
        << testEnemy.getHP()
        << "/"
        << testEnemy.getMaxHP()
        << "\n";

    std::cout << "\n";
    std::cout << "===== USING HEAL ALLY =====\n";

    // Damage Crusader first so we can see the heal work
    crusader.takeDamage(10);

    std::cout
        << "Crusader HP before healing: "
        << crusader.getHP()
        << "/"
        << crusader.getMaxHP()
        << "\n";

    crusader.useSkill(2, crusader);

    std::cout
        << "Crusader HP after healing: "
        << crusader.getHP()
        << "/"
        << crusader.getMaxHP()
        << "\n";

    /*
        Battle test comes after the skill test.
    */

    Battle battle(playerParty);

    battle.start();

    return 0;
}