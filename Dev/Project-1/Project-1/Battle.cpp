#include "Battle.h"

#include <iostream>
#include <random>

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");

#endif
}

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
    const std::string GREEN = "\033[32m";
    const std::string CRIMSON = "\033[38;5;88m";
    const std::string RESET = "\033[0m";

    std::cout << "\n";
    std::cout << "================================================================================\n";
    std::cout << "                                  BATTLE\n";
    std::cout << "================================================================================\n\n";

    std::cout << GREEN
        << "PLAYER PARTY"
        << RESET;

    std::cout << "                                  ";

    std::cout << CRIMSON
        << "ENEMY PARTY"
        << RESET;

    std::cout << "\n";

    std::cout << "--------------------------------------------------------------------------------\n";

    for (int i = 1; i <= 4; i++)
    {
        Character* player = playerParty.getMember(i);
        Character* enemy = enemyParty.getMember(i);

        // Player name
        std::string playerName = "Empty";

        if (player != nullptr)
        {
            playerName = player->getName();
        }

        // Enemy name
        std::string enemyName = "Empty";

        if (enemy != nullptr)
        {
            enemyName = enemy->getName();
        }

        // Names
        std::cout << GREEN
            << i << ". " << playerName
            << RESET;

        // Fixed spacing between columns
        std::cout << std::string(35 - playerName.length(), ' ');

        std::cout << CRIMSON
            << i << ". " << enemyName
            << RESET;

        std::cout << "\n";

        // Player HP / Stress
        if (player != nullptr)
        {
            std::cout << GREEN
                << "   HP: "
                << player->getHP()
                << "/"
                << player->getMaxHP()
                << "   Stress: "
                << player->getStress()
                << "/200"
                << RESET;
        }
        else
        {
            std::cout << GREEN
                << "   HP: --   Stress: --"
                << RESET;
        }

        // Fixed spacing
        std::cout << std::string(35, ' ');

        // Enemy HP
        if (enemy != nullptr)
        {
            std::cout << CRIMSON
                << "   HP: "
                << enemy->getHP()
                << "/"
                << enemy->getMaxHP()
                << RESET;
        }
        else
        {
            std::cout << CRIMSON
                << "   HP: --"
                << RESET;
        }

        std::cout << "\n";

        // Player position
        if (player != nullptr)
        {
            std::cout << GREEN
                << "   Position: "
                << player->getPosition()
                << RESET;
        }
        else
        {
            std::cout << GREEN
                << "   Position: --"
                << RESET;
        }

        // Fixed spacing
        std::cout << std::string(35, ' ');

        // Enemy position
        if (enemy != nullptr)
        {
            std::cout << CRIMSON
                << "   Position: "
                << enemy->getPosition()
                << RESET;
        }
        else
        {
            std::cout << CRIMSON
                << "   Position: --"
                << RESET;
        }

        std::cout << "\n\n";
    }

    std::cout << "================================================================================\n";
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

Character* Battle::chooseEnemyTarget(
    Character& character,
    int minimumPosition,
    int maximumPosition)
{
    while (true)
    {
        std::cout << "\n";
        std::cout << "Choose a target:\n";

        bool hasTarget = false;

        for (int i = 1; i <= 4; i++)
        {
            Character* enemy = enemyParty.getMember(i);

            if (enemy == nullptr)
                continue;

            if (!enemy->isAlive())
                continue;

            if (!character.isTargetInRange(*enemy, minimumPosition, maximumPosition))
                continue;

            std::cout
                << i << ". "
                << enemy->getName()
                << " (" << enemy->getHP()
                << "/" << enemy->getMaxHP()
                << " HP)\n";

            hasTarget = true;
        }

        if (!hasTarget)
        {
            std::cout << "No enemies are in range.\n";
            return nullptr;
        }

        std::cout << "0. Back\n";
        std::cout << "Choose a target: ";

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
            return nullptr;

        Character* target = enemyParty.getMember(choice);

        if (target == nullptr ||
            !target->isAlive() ||
            !character.isTargetInRange(*target, minimumPosition, maximumPosition))
        {
            std::cout << "Invalid target.\n";
            continue;
        }

        return target;
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

bool Battle::attackMenu(Character& character)
{
    Character* target = nullptr;

    if (character.getName() == "Crusader")
        target = chooseEnemyTarget(character, 1, 2);
    else if (character.getName() == "Highwayman")
        target = chooseEnemyTarget(character, 2, 4);
    else if (character.getName() == "Plague Doctor")
        target = chooseEnemyTarget(character, 1, 2);
    else if (character.getName() == "Vestal")
        target = chooseEnemyTarget(character, 1, 2);

    if (target == nullptr)
        return false;

    character.attack(*target);
    return true;
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
            int minimumPosition = 1;
            int maximumPosition = 4;

            if (character.getName() == "Crusader")
            {
                if (choice == 1) // Smite
                {
                    minimumPosition = 3;
                    maximumPosition = 4;
                }
            }
            else if (character.getName() == "Highwayman")
            {
                if (choice == 1) // Pistol Shot
                {
                    minimumPosition = 2;
                    maximumPosition = 4;
                }
                else if (choice == 2) // Melee Attack
                {
                    minimumPosition = 1;
                    maximumPosition = 2;
                }
            }
            else if (character.getName() == "Plague Doctor")
            {
                if (choice == 1) // Plague Grenade
                {
                    minimumPosition = 3;
                    maximumPosition = 4;
                }
            }
            else if (character.getName() == "Vestal")
            {
                if (choice == 1) // Smite
                {
                    minimumPosition = 1;
                    maximumPosition = 2;
                }
            }

            target = chooseEnemyTarget(
                character,
                minimumPosition,
                maximumPosition
            );
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
    while (true)
    {
        clearScreen();

        printBattleState();

        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << character.getName() << "'s Turn\n";
        std::cout << "====================================\n";

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

            std::cout << "Invalid input.\n";
            continue;
        }

        if (choice == 1)
        {
            attackMenu(character);
            return;
        }

        if (choice == 2)
        {
            skillMenu(character);
            return;
        }

        if (choice == 3)
        {
            continue;
        }

        if (choice == 4)
        {
            std::cout << character.getName() << " passes.\n";
            return;
        }

        std::cout << "Invalid choice.\n";
    }
}

void Battle::enemyTurn(Character& enemy)
{

    clearScreen();

    printBattleState();

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