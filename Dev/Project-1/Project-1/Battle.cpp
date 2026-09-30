#include "Battle.h"

#include <iostream>
#include <random>

Battle::Battle(Party& playerParty)
    : playerParty(playerParty),
    enemyCount(0)
{
    for (int i = 0; i < 4; i++)
    {
        enemyRoster[i] = nullptr;
    }

    generateEnemyParty();
}

Battle::~Battle()
{
    for (int i = 0; i < 4; i++)
    {
        delete enemyRoster[i];
        enemyRoster[i] = nullptr;
    }
}

void Battle::generateEnemyParty()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> countDistribution(1, 4);

    enemyCount = countDistribution(generator);

    std::cout << "\n";
    std::cout << "Generating enemy party...\n";
    std::cout << "Enemy count: "
        << enemyCount
        << "\n";

    for (int i = 0; i < enemyCount; i++)
    {
        enemyRoster[i] = createRandomEnemy();

        if (enemyRoster[i] != nullptr)
        {
            enemyParty.addMember(
                *enemyRoster[i],
                i + 1
            );
        }
    }
}

Enemy* Battle::createRandomEnemy()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> enemyDistribution(1, 3);

    int enemyType = enemyDistribution(generator);

    switch (enemyType)
    {
    case 1:
    {
        return new Enemy(
            "Brigand",
            {
                20,
                4,
                80,
                3,
                6,
                5
            },
            10,
            20
        );
    }

    case 2:
    {
        return new Enemy(
            "Cultist",
            {
                18,
                5,
                85,
                2,
                5,
                8
            },
            12,
            25
        );
    }

    case 3:
    {
        return new Enemy(
            "Ghoul",
            {
                28,
                3,
                75,
                5,
                9,
                3
            },
            15,
            30
        );
    }
    }

    return nullptr;
}

void Battle::printCharacterStatus(
    const Character& character
) const
{
    std::cout
        << character.getName()
        << "  HP: "
        << character.getHP()
        << "/"
        << character.getMaxHP()
        << "  Position: "
        << character.getPosition()
        << "\n";
}

void Battle::printBattleState() const
{
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "              BATTLE\n";
    std::cout << "====================================\n";

    std::cout << "\n";
    std::cout << "----------- YOUR PARTY ------------\n";

    for (int i = 1; i <= 4; i++)
    {
        Character* character =
            playerParty.getMember(i);

        if (character != nullptr)
        {
            std::cout << "[" << i << "] ";
            printCharacterStatus(*character);
        }
        else
        {
            std::cout
                << "["
                << i
                << "] EMPTY\n";
        }
    }

    std::cout << "\n";
    std::cout << "------------ ENEMIES --------------\n";

    for (int i = 1; i <= 4; i++)
    {
        Character* enemy =
            enemyParty.getMember(i);

        if (enemy != nullptr)
        {
            std::cout << "[" << i << "] ";

            if (enemy->isAlive())
            {
                printCharacterStatus(*enemy);
            }
            else
            {
                std::cout
                    << enemy->getName()
                    << "  DEAD\n";
            }
        }
    }

    std::cout << "====================================\n";
}

bool Battle::playerPartyAlive() const
{
    for (int i = 1; i <= 4; i++)
    {
        Character* character =
            playerParty.getMember(i);

        if (character != nullptr &&
            character->isAlive())
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
        Character* enemy =
            enemyParty.getMember(i);

        if (enemy != nullptr &&
            enemy->isAlive())
        {
            return true;
        }
    }

    return false;
}

Character* Battle::chooseEnemyTarget()
{
    while (true)
    {
        std::cout << "\n";
        std::cout << "Choose a target:\n";

        for (int i = 1; i <= 4; i++)
        {
            Character* enemy =
                enemyParty.getMember(i);

            if (enemy != nullptr &&
                enemy->isAlive())
            {
                std::cout
                    << i
                    << ". "
                    << enemy->getName()
                    << " ("
                    << enemy->getHP()
                    << "/"
                    << enemy->getMaxHP()
                    << " HP)\n";
            }
        }


        std::cout << "0. Back\n";
        std::cout << "\n";
        std::cout << "Choose: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');

            std::cout
                << "Invalid input.\n";

            continue;
        }

        if (choice == 0)
        {
            return nullptr;
        }

        if (choice >= 1 && choice <= 4)
        {
            Character* target =
                enemyParty.getMember(choice);

            if (target != nullptr &&
                target->isAlive())
            {
                return target;
            }
        }

        std::cout
            << "Invalid target.\n";
    }
}
Character* Battle::chooseAllyTarget()
{
    while (true)
    {
        std::cout << "\n";
        std::cout << "Choose an ally:\n";

        for (int i = 1; i <= 4; i++)
        {
            Character* ally =
                playerParty.getMember(i);

            if (ally != nullptr &&
                ally->isAlive())
            {
                std::cout
                    << i
                    << ". "
                    << ally->getName()
                    << " ("
                    << ally->getHP()
                    << "/"
                    << ally->getMaxHP()
                    << " HP)\n";
            }
        }

        std::cout << "0. Back\n";
        std::cout << "\n";
        std::cout << "Choose: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');

            std::cout
                << "Invalid input.\n";

            continue;
        }

        if (choice == 0)
        {
            return nullptr;
        }

        if (choice >= 1 && choice <= 4)
        {
            Character* ally =
                playerParty.getMember(choice);

            if (ally != nullptr &&
                ally->isAlive())
            {
                return ally;
            }
        }

        std::cout
            << "Invalid target.\n";
    }
}

void Battle::attackMenu(Character& character)
{
    Character* target =
        chooseEnemyTarget();

    if (target == nullptr)
    {
        return;
    }

    character.attack(*target);
}
void Battle::skillMenu(Character& character)
{
    while (true)
    {
        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout
            << character.getName()
            << " Skills\n";
        std::cout << "====================================\n";

        character.printSkills();

        std::cout << "0. Back\n";

        std::cout << "\n";
        std::cout << "Choose a skill: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');

            std::cout << "Invalid input.\n";

            continue;
        }

        if (choice == 0)
        {
            return;
        }

        if (choice < 1)
        {
            std::cout << "Invalid skill.\n";
            continue;
        }

        TargetType targetType =
            character.getSkillTargetType(choice);

        Character* target = nullptr;

        if (targetType == TargetType::Enemy)
        {
            target = chooseEnemyTarget();
        }
        else if (targetType == TargetType::Ally)
        {
            target = chooseAllyTarget();
        }

        if (target == nullptr)
        {
            continue;
        }

        character.useSkill(choice, *target);

        return;
    }
}
void Battle::playerTurn(Character& character)
{
    while (character.isAlive())
    {
        std::cout << "\n";
        std::cout
            << "====================================\n";

        std::cout
            << character.getName()
            << "'s Turn\n";

        std::cout
            << "HP: "
            << character.getHP()
            << "/"
            << character.getMaxHP()
            << "\n";

        std::cout
            << "====================================\n";

        std::cout << "1. Attack\n";
        std::cout << "2. Skills\n";
        std::cout << "3. Party Status\n";
        std::cout << "4. Pass\n";

        std::cout << "\n";
        std::cout << "Choose an action: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');

            std::cout
                << "Invalid input.\n";

            continue;
        }

        switch (choice)
        {
        case 1:
        {
            attackMenu(character);

            return;
        }

        case 2:
        {
            skillMenu(character);

            return;
        }

        case 3:
        {
            printBattleState();
            break;
        }

        case 4:
        {
            std::cout
                << character.getName()
                << " passes their turn.\n";

            return;
        }

        default:
        {
            std::cout
                << "Invalid choice.\n";

            break;
        }
        }
    }
}

void Battle::enemyTurn(Character& enemy)
{
    if (!enemy.isAlive())
    {
        return;
    }

    std::cout << "\n";
    std::cout
        << "------------------------------------\n";

    std::cout
        << enemy.getName()
        << "'s turn!\n";

    std::cout
        << "------------------------------------\n";

    /*
        Temporary enemy AI.

        For now the enemy simply attacks
        a random living hero.
    */

    Character* targets[4];
    int targetCount = 0;

    for (int i = 1; i <= 4; i++)
    {
        Character* target =
            playerParty.getMember(i);

        if (target != nullptr &&
            target->isAlive())
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

    std::uniform_int_distribution<int>
        targetDistribution(
            0,
            targetCount - 1
        );

    Character* target =
        targets[targetDistribution(generator)];

    enemy.attack(*target);
}

void Battle::start()
{
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "          BATTLE START!\n";
    std::cout << "====================================\n";

    printBattleState();

    while (playerPartyAlive() &&
        enemyPartyAlive())
    {
        /*
            Player turns.

            For now, heroes act in formation
            order: 1 -> 2 -> 3 -> 4.
        */

        for (int i = 1; i <= 4; i++)
        {
            if (!enemyPartyAlive())
            {
                break;
            }

            Character* character =
                playerParty.getMember(i);

            if (character != nullptr &&
                character->isAlive())
            {
                playerTurn(*character);
            }
        }

        if (!enemyPartyAlive())
        {
            break;
        }

        /*
            Enemy turns.

            For now, enemies also act in
            formation order.
        */

        for (int i = 1; i <= 4; i++)
        {
            if (!playerPartyAlive())
            {
                break;
            }

            Character* enemy =
                enemyParty.getMember(i);

            if (enemy != nullptr &&
                enemy->isAlive())
            {
                enemyTurn(*enemy);
            }
        }

        printBattleState();
    }

    std::cout << "\n";
    std::cout << "====================================\n";

    if (playerPartyAlive())
    {
        std::cout
            << "           VICTORY!\n";
    }
    else
    {
        std::cout
            << "            DEFEAT!\n";
    }

    std::cout << "====================================\n";
}

Party& Battle::getPlayerParty()
{
    return playerParty;
}

Party& Battle::getEnemyParty()
{
    return enemyParty;
}