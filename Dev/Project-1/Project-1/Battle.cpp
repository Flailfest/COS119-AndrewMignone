
#include "Battle.h"
#include "Input.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

// --------------------------------------------------
// Position rules
// --------------------------------------------------

namespace
{
    struct PositionRules
    {
        int userMin;
        int userMax;
        int targetMin;
        int targetMax;
    };

    bool getAttackRules(
        const Character& character,
        PositionRules& rules)
    {
        const std::string name = character.getName();

        if (name == "Crusader")
        {
            rules = { 1, 2, 1, 2 };
            return true;
        }

        if (name == "Highwayman")
        {
            rules = { 1, 3, 2, 4 };
            return true;
        }

        if (name == "Plague Doctor")
        {
            rules = { 1, 3, 1, 2 };
            return true;
        }

        if (name == "Vestal")
        {
            rules = { 1, 4, 1, 2 };
            return true;
        }

        return false;
    }

    bool getSkillRules(
        const Character& character,
        int skillNumber,
        PositionRules& rules)
    {
        const std::string name = character.getName();

        // Default rules for skills that target allies.
        rules = { 1, 4, 1, 4 };

        if (name == "Crusader")
        {
            if (skillNumber == 1) // Holy Strike
            {
                rules = { 1, 2, 3, 4 };
                return true;
            }

            if (skillNumber == 2) // Heal Ally
            {
                return true;
            }
        }

        else if (name == "Highwayman")
        {
            if (skillNumber == 1) // Pistol Shot
            {
                rules = { 1, 3, 2, 4 };
                return true;
            }

            if (skillNumber == 2) // Melee Attack
            {
                rules = { 1, 2, 1, 2 };
                return true;
            }
        }

        else if (name == "Plague Doctor")
        {
            if (skillNumber == 1) // Plague Grenade
            {
                rules = { 1, 2, 3, 4 };
                return true;
            }

            if (skillNumber == 2) // Heal Ally
            {
                rules = { 1, 4, 1, 4 };
                return true;
            }
        }

        else if (name == "Vestal")
        {
            if (skillNumber == 1) // Smite
            {
                rules = { 1, 4, 1, 2 };
                return true;
            }

            if (skillNumber == 2) // Heal Ally
            {
                rules = { 1, 4, 1, 4 };
                return true;
            }
        }

        return false;
    }

    bool isPositionInRange(
        int position,
        int minimumPosition,
        int maximumPosition)
    {
        return position >= minimumPosition &&
            position <= maximumPosition;
    }

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

        std::uniform_int_distribution<int> distribution(
            minimum,
            maximum
        );

        return distribution(generator);
    }

    void printColumnGap(const std::string& text)
    {
        const int columnWidth = 45;
        int spaces = columnWidth - static_cast<int>(text.length());

        if (spaces < 1)
        {
            spaces = 1;
        }

        std::cout << std::string(spaces, ' ');
    }
}

// --------------------------------------------------
// Screen clearing
// --------------------------------------------------

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// --------------------------------------------------
// Constructor / destructor
// --------------------------------------------------

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

// --------------------------------------------------
// Enemy generation
// --------------------------------------------------

void Battle::generateEnemyParty()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> countDistribution(1, 4);

    enemyCount = countDistribution(generator);

    std::cout << "\n";
    std::cout << "Generating enemy party...\n";
    std::cout << "Enemy count: " << enemyCount << "\n";

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
        return new Enemy(
            "Brigand",
            {
                20, 4, 80, 3, 6, 5
            },
            10,
            20
        );

    case 2:
        return new Enemy(
            "Cultist",
            {
                18, 5, 85, 2, 5, 8
            },
            12,
            25
        );

    case 3:
        return new Enemy(
            "Ghoul",
            {
                28, 3, 75, 5, 9, 3
            },
            15,
            30
        );
    }

    return nullptr;
}

// --------------------------------------------------
// Battle display
// --------------------------------------------------

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

    const int columnWidth = 45;

    std::cout << "\n";
    std::cout << "================================================================================\n";
    std::cout << "                                  BATTLE\n";
    std::cout << "================================================================================\n\n";

    std::cout
        << GREEN
        << std::left
        << std::setw(columnWidth)
        << "PLAYER PARTY"
        << RESET;

    std::cout << CRIMSON << "ENEMY PARTY" << RESET;
    std::cout << "\n";

    std::cout << "--------------------------------------------------------------------------------\n";

    for (int i = 1; i <= 4; i++)
    {
        Character* player = playerParty.getMember(i);
        Character* enemy = enemyParty.getMember(i);

        // Names
        std::string playerName;
        std::string enemyName;

        if (player != nullptr)
        {
            playerName =
                std::to_string(i) + ". " + player->getName();
        }

        if (enemy != nullptr)
        {
            enemyName =
                std::to_string(i) + ". " + enemy->getName();
        }

        if (player != nullptr)
        {
            std::cout << GREEN
                << std::left
                << playerName
                << RESET;
        }

        printColumnGap(playerName);

        // Only display occupied enemy positions.
        if (enemy != nullptr)
        {
            std::cout << CRIMSON
                << enemyName
                << RESET;
        }

        std::cout << "\n";

        // Player HP / stress
        std::string playerStats;

        if (player != nullptr)
        {
            playerStats =
                "   HP: " +
                std::to_string(player->getHP()) +
                "/" +
                std::to_string(player->getMaxHP()) +
                "   Stress: " +
                std::to_string(player->getStress()) +
                "/200";

            std::cout << GREEN << playerStats << RESET;
        }

        printColumnGap(playerStats);

        // Enemy HP
        if (enemy != nullptr)
        {
            std::string enemyStats =
                "   HP: " +
                std::to_string(enemy->getHP()) +
                "/" +
                std::to_string(enemy->getMaxHP());

            std::cout << CRIMSON << enemyStats << RESET;
        }

        std::cout << "\n";

        // Player position
        std::string playerPosition;

        if (player != nullptr)
        {
            playerPosition =
                "   Position: " +
                std::to_string(playerParty.getPosition(*player));

            std::cout << GREEN << playerPosition << RESET;
        }

        printColumnGap(playerPosition);

        // Enemy position
        if (enemy != nullptr)
        {
            std::string enemyPosition =
                "   Position: " +
                std::to_string(enemyParty.getPosition(*enemy));

            std::cout << CRIMSON << enemyPosition << RESET;
        }

        std::cout << "\n\n";
    }

    std::cout << "================================================================================\n";
}

// --------------------------------------------------
// Party alive checks
// --------------------------------------------------

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

// --------------------------------------------------
// Enemy target selection
// --------------------------------------------------

Character* Battle::chooseEnemyTarget(
    Character& character,
    int minimumPosition,
    int maximumPosition)
{
    while (true)
    {
        std::cout << "\n";
        std::cout << "Choose a target for "
            << character.getName()
            << ":\n";

        bool hasTarget = false;

        for (int i = 1; i <= 4; i++)
        {
            Character* enemy = enemyParty.getMember(i);

            if (enemy == nullptr || !enemy->isAlive())
            {
                continue;
            }

            int enemyPosition = enemyParty.getPosition(*enemy);

            if (!isPositionInRange(
                enemyPosition,
                minimumPosition,
                maximumPosition))
            {
                continue;
            }

            std::cout
                << enemyPosition
                << ". "
                << enemy->getName()
                << " ("
                << enemy->getHP()
                << "/"
                << enemy->getMaxHP()
                << " HP)\n";

            hasTarget = true;
        }

        if (!hasTarget)
        {
            std::cout << "No enemies are in range.\n";
            return nullptr;
        }

        std::cout << "0. Back\n";

        int choice = Input::getInt(
            "Choose a target: "
        );

        if (choice == 0)
        {
            return nullptr;
        }

        Character* target = enemyParty.getMember(choice);

        if (target == nullptr || !target->isAlive())
        {
            std::cout << "Invalid target.\n";
            continue;
        }

        int targetPosition = enemyParty.getPosition(*target);

        if (!isPositionInRange(
            targetPosition,
            minimumPosition,
            maximumPosition))
        {
            std::cout << "That target is out of range.\n";
            continue;
        }

        return target;
    }
}

// --------------------------------------------------
// Ally target selection
// --------------------------------------------------

Character* Battle::chooseAllyTarget()
{
    while (true)
    {
        std::cout << "\n";
        std::cout << "Choose an ally:\n";

        for (int i = 1; i <= 4; i++)
        {
            Character* ally = playerParty.getMember(i);

            if (ally == nullptr || !ally->isAlive())
            {
                continue;
            }

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

        std::cout << "0. Back\n";

        int choice = Input::getInt(
            "Choose an ally: "
        );

        if (choice == 0)
        {
            return nullptr;
        }

        Character* target = playerParty.getMember(choice);

        if (target == nullptr || !target->isAlive())
        {
            std::cout << "Invalid target.\n";
            continue;
        }

        return target;
    }
}

// --------------------------------------------------
// Basic attack menu
// --------------------------------------------------

bool Battle::attackMenu(Character& character)
{
    PositionRules rules;

    if (!getAttackRules(character, rules))
    {
        std::cout << "No basic attack rules exist for this character.\n";
        Input::waitForEnter();
        return false;
    }

    // Check the attacking character's formation position.
    int userPosition = playerParty.getPosition(character);

    if (!isPositionInRange(
        userPosition,
        rules.userMin,
        rules.userMax))
    {
        std::cout
            << character.getName()
            << " cannot attack from position "
            << userPosition
            << ".\n";

        Input::waitForEnter();
        return false;
    }

    // Only show enemies that the basic attack can reach.
    Character* target = chooseEnemyTarget(
        character,
        rules.targetMin,
        rules.targetMax
    );

    if (target == nullptr)
    {
        return false;
    }

    // Call the base attack directly.
    // Battle has already checked the user's and target's positions.
    character.Character::attack(*target);

    return true;
}

// --------------------------------------------------
// Skill menu
// --------------------------------------------------

void Battle::skillMenu(Character& character)
{
    while (true)
    {
        clearScreen();
        printBattleState();

        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << character.getName() << " Skills\n";
        std::cout << "====================================\n";

        character.printSkills();

        std::cout << "0. Back\n";

        int choice = Input::getInt(
            "\nChoose a skill: "
        );

        if (choice == 0)
        {
            return;
        }

        PositionRules rules;

        if (!getSkillRules(character, choice, rules))
        {
            std::cout << "Invalid skill.\n";
            Input::waitForEnter();
            continue;
        }

        // Check the user's formation position before selecting a target.
        int userPosition = playerParty.getPosition(character);

        if (!isPositionInRange(
            userPosition,
            rules.userMin,
            rules.userMax))
        {
            std::cout
                << character.getName()
                << " cannot use that skill from position "
                << userPosition
                << ".\n";

            Input::waitForEnter();
            continue;
        }

        TargetType targetType =
            character.getSkillTargetType(choice);

        Character* target = nullptr;

        if (targetType == TargetType::Enemy)
        {
            target = chooseEnemyTarget(
                character,
                rules.targetMin,
                rules.targetMax
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

        // The corresponding character skill performs the actual effect.
        character.useSkill(choice, *target);

        return;
    }
}

// --------------------------------------------------
// Player turn
// --------------------------------------------------

void Battle::playerTurn(Character& character)
{
    // Stress is rolled once per turn, not once per menu redraw.
    bool gainedStress = false;
    int stressIncrease = 0;

    if (stressOccurs())
    {
        stressIncrease = randomStress(1, 3);
        character.addStress(stressIncrease);
        gainedStress = true;
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
        std::cout << "3. Party Status\n";
        std::cout << "4. Pass\n";

        int choice = Input::getInt(
            "\nChoose an action: "
        );

        if (choice == 1)
        {
            if (attackMenu(character))
            {
                return;
            }

            // Back or invalid position does not consume the turn.
            continue;
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

// --------------------------------------------------
// Enemy turn
// --------------------------------------------------

void Battle::enemyTurn(Character& enemy)
{
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

    // Collect living player characters.
    Character* targets[4];
    int targetCount = 0;

    for (int i = 1; i <= 4; i++)
    {
        Character* target = playerParty.getMember(i);

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

    enemy.attack(*target);

    // Only player characters gain stress when attacked.
    int stressIncrease = randomStress(1, 4);
    target->addStress(stressIncrease);

    std::cout
        << target->getName()
        << " gains "
        << stressIncrease
        << " stress from the attack.\n";
}

// --------------------------------------------------
// Start battle
// --------------------------------------------------

void Battle::start()
{
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "          BATTLE START!\n";
    std::cout << "====================================\n";

    printBattleState();

    while (playerPartyAlive() && enemyPartyAlive())
    {
        // Player characters act in formation order.
        for (int i = 1; i <= 4; i++)
        {
            if (!enemyPartyAlive())
            {
                break;
            }

            Character* character = playerParty.getMember(i);

            if (character != nullptr && character->isAlive())
            {
                playerTurn(*character);
            }
        }

        if (!enemyPartyAlive())
        {
            break;
        }

        // Enemies act in formation order.
        for (int i = 1; i <= 4; i++)
        {
            if (!playerPartyAlive())
            {
                break;
            }

            Character* enemy = enemyParty.getMember(i);

            if (enemy != nullptr && enemy->isAlive())
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
        std::cout << "           VICTORY!\n";
    }
    else
    {
        std::cout << "            DEFEAT!\n";
    }

    std::cout << "====================================\n";
}

// --------------------------------------------------
// Party accessors
// --------------------------------------------------

Party& Battle::getPlayerParty()
{
    return playerParty;
}

Party& Battle::getEnemyParty()
{
    return enemyParty;
}
