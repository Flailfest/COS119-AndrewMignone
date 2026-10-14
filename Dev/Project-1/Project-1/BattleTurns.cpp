#include "Battle.h"
#include "BattleUI.h"
#include "Input.h"

#include <iostream>
#include <random>
#include <string>

namespace
{
    bool stressOccurs()
    {
        static std::random_device rd;
        static std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(1, 100);
        return distribution(generator) <= 40;
    }

    int randomStress(int minimum, int maximum)
    {
        static std::random_device rd;
        static std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(minimum, maximum);
        return distribution(generator);
    }

std::string getRandomNarration(bool playerTurn)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static const std::string playerLines[] =
    {
        "The cellar holds its breath.",
        "Courage falters, but the fight continues.",
        "Steel may yet prevail against this darkness.",
        "Every step forward demands a price.",
        "The shadows stir around the living.",
        "There is no safety here. Only survival.",
        "Resolve is tested in the face of horror.",
        "The darkness waits for a moment of weakness."
    };

    static const std::string enemyLines[] =
    {
        "Something vile stirs in the darkness.",
        "The cellar hungers for another soul.",
        "A foul presence moves against the party.",
        "The darkness strikes without mercy.",
        "The creatures of this place know no fear.",
        "The stench of death grows stronger.",
        "An unnatural silence precedes the violence.",
        "The horrors here will not yield easily."
    };
    const int phraseCount = 8;
    std::uniform_int_distribution<int> distribution(
        0,
        phraseCount - 1
    );

    int index = distribution(generator);
    if (playerTurn)
    {
        return playerLines[index];
    }

    return enemyLines[index];
}

}

bool Battle::playerPartyAlive() const
{
    for (int i = 1; i <= 4; i++)
    {
        Character* character = playerParty.getMember(i);

        if (character != nullptr && character->isAlive())
        {
            return true;
        }
    }

    return false;
}


bool Battle::enemyPartyAlive() const
{
    for (int i = 1; i <= 4; i++)
    {
        Character* enemy = enemyParty.getMember(i);

        if (enemy != nullptr && enemy->isAlive())
        {
            return true;
        }
    }

    return false;
}


bool Battle::resolveStressReaction(Character& character)
{
    if (character.getStress() <= 100)
    {
        return false;
    }
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> chanceDistribution(
        1,
        100
    );

    if (chanceDistribution(generator) > 30)
    {
        return false;
    }
    std::uniform_int_distribution<int> reactionDistribution(
        1,
        3
    );

    int reaction = reactionDistribution(generator);
    if (reaction == 1)
    {
        Character* targets[4];
        int targetCount = 0;
        for (int i = 1; i <= 4; i++)
        {
            Character* enemy = enemyParty.getMember(i);

            if (enemy != nullptr && enemy->isAlive())
            {
                targets[targetCount] = enemy;
                targetCount++;
            }
        }
        if (targetCount == 0)
        {
            return false;
        }
        std::uniform_int_distribution<int> targetDistribution(
            0,
            targetCount - 1
        );

        Character* target =
            targets[targetDistribution(generator)];
        int hpBefore = target->getHP();
        character.Character::attack(*target);
        int damageDealt = hpBefore - target->getHP();
        lastActionMessage =
            character.getName() +
            " loses control and attacks " +
            target->getName() +
            " for " +
            std::to_string(damageDealt) +
            " damage!";

        narratorMessage =
            "Desperation overwhelms discipline.";
        if (!target->isAlive())
        {
            lastActionMessage +=
                " " +
                target->getName() +
                " has been defeated!";
        }

        return true;
    }
    if (reaction == 2)
    {
        lastActionMessage =
            character.getName() +
            " is overwhelmed by stress and refuses to act!";

        narratorMessage =
            "Fear steals the will to act.";

        return true;
    }
    Character* allies[4];
    int allyCount = 0;

    for (int i = 1; i <= 4; i++)
    {
        Character* ally = playerParty.getMember(i);

        if (ally != nullptr &&
            ally->isAlive() &&
            ally != &character)
        {
            allies[allyCount] = ally;
            allyCount++;
        }
    }
    if (allyCount == 0)
    {
        lastActionMessage =
            character.getName() +
            " is overwhelmed by stress and cannot act!";

        narratorMessage =
            "The mind collapses beneath the strain.";

        return true;
    }
    std::uniform_int_distribution<int> allyDistribution(
        0,
        allyCount - 1
    );

    Character* target =
        allies[allyDistribution(generator)];
    int hpBefore = target->getHP();
    character.Character::attack(*target);

    int damageDealt = hpBefore - target->getHP();
    int stressIncrease = randomStress(1, 4);
    target->addStress(stressIncrease);
    lastActionMessage =
        character.getName() +
        " loses control and attacks ally " +
        target->getName() +
        " for " +
        std::to_string(damageDealt) +
        " damage! " +
        target->getName() +
        " gains " +
        std::to_string(stressIncrease) +
        " stress.";

    narratorMessage =
        "Fear turns companions against one another.";

    if (!target->isAlive())
    {
        lastActionMessage +=
            " " +
            target->getName() +
            " has been defeated!";
    }

    return true;
}


void Battle::playerTurn(Character& character)
{
    narratorMessage = getRandomNarration(true);
    bool gainedStress = false;
    int stressIncrease = 0;

    if (stressOccurs())
    {
        stressIncrease = randomStress(1, 3);
        character.addStress(stressIncrease);
        gainedStress = true;
    }
    if (resolveStressReaction(character))
    {
        clearScreen();
        printBattleState();

        Input::waitForEnter();
        return;
    }

    while (true)
    {
        clearScreen();
        printBattleState();

        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << character.getName() << "'s Turn\n";
        std::cout << "====================================\n";
        if (gainedStress)
        {
            std::cout
                << character.getName()
                << " gains "
                << stressIncrease
                << " stress.\n";
        }
        else
        {
            std::cout
                << character.getName()
                << " does not gain stress this turn.\n";
        }
        std::cout << "\n";
        std::cout << "1. Attack\n";
        std::cout << "2. Skills\n";
        std::cout << "3. Move Party Member\n";
        std::cout << "4. Pass\n";

        int choice = Input::getIntInRange(
            "\nChoose an action: ",
            1,
            4
        );
        if (choice == 1)
        {
            if (attackMenu(character))
            {
                return;
            }

            continue;
        }
        if (choice == 2)
        {
            skillMenu(character);
            return;
        }
        if (choice == 3)
        {
            if (movePartyMember())
            {
                return;
            }

            continue;
        }
        if (choice == 4)
        {
            std::cout
                << character.getName()
                << " passes.\n";

            return;
        }
    }
}


void Battle::enemyTurn(Character& enemy)
{
    narratorMessage = getRandomNarration(false);
    clearScreen();
    printBattleState();
    if (!enemy.isAlive())
    {
        return;
    }
    std::cout << "\n";
    std::cout << "------------------------------------\n";
    std::cout << enemy.getName() << "'s turn!\n";
    std::cout << "------------------------------------\n";
    Character* targets[4];
    int targetCount = 0;

    for (int position = 1; position <= 4; position++)
    {
        Character* target =
            playerParty.getMember(position);
        if (target != nullptr && target->isAlive())
        {
            targets[targetCount] = target;
            targetCount++;
        }
    }
    if (targetCount == 0)
    {
        return;
    }
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<int> targetDistribution(
        0,
        targetCount - 1
    );

    Character* target =
        targets[targetDistribution(generator)];
    int hpBefore = target->getHP();
    enemy.attack(*target);
    int damageDealt = hpBefore - target->getHP();
    int stressIncrease = randomStress(1, 4);

    target->addStress(stressIncrease);
    lastActionMessage =
        enemy.getName() +
        " attacks " +
        target->getName() +
        " for " +
        std::to_string(damageDealt) +
        " damage. " +
        target->getName() +
        " gains " +
        std::to_string(stressIncrease) +
        " stress.";
    if (!target->isAlive())
    {
        lastActionMessage +=
            " " +
            target->getName() +
            " has been defeated!";
    }
    if (target->isAlive())
    {
        std::uniform_int_distribution<int> displacementChance(
            1,
            100
        );

        if (displacementChance(generator) <= 25)
        {
            int fromPosition =
                playerParty.getPosition(*target);
            std::uniform_int_distribution<int> positionDistribution(
                1,
                4
            );

            int toPosition;

            do
            {
                toPosition = positionDistribution(generator);
            } while (toPosition == fromPosition);
            Character* displacedCharacter =
                playerParty.getMember(toPosition);
            bool moved = playerParty.moveMember(
                fromPosition,
                toPosition
            );
            if (moved)
            {
                if (displacedCharacter != nullptr)
                {
                    lastActionMessage +=
                        " " +
                        target->getName() +
                        " is knocked from position " +
                        std::to_string(fromPosition) +
                        " to position " +
                        std::to_string(toPosition) +
                        ", swapping places with " +
                        displacedCharacter->getName() +
                        ".";
                }
                else
                {
                    lastActionMessage +=
                        " " +
                        target->getName() +
                        " is knocked from position " +
                        std::to_string(fromPosition) +
                        " to position " +
                        std::to_string(toPosition) +
                        ".";
                }
            }
        }
    }
}

