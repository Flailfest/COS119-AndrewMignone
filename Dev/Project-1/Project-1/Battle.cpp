#include "Battle.h"
#include "Input.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

// 1.1
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

        // 1.2
        rules = { 1, 4, 1, 4 };

        if (name == "Crusader")
        {
            if (skillNumber == 1) // 1.3
            {
                rules = { 1, 2, 3, 4 };
                return true;
            }

            if (skillNumber == 2) // 1.4
            {
                return true;
            }
        }

        else if (name == "Highwayman")
        {
            if (skillNumber == 1) // 1.5
            {
                rules = { 1, 3, 2, 4 };
                return true;
            }

            if (skillNumber == 2) // 1.6
            {
                rules = { 1, 2, 1, 2 };
                return true;
            }
        }

        else if (name == "Plague Doctor")
        {
            if (skillNumber == 1) // 1.7
            {
                rules = { 1, 2, 3, 4 };
                return true;
            }

            if (skillNumber == 2) // 1.8
            {
                rules = { 1, 4, 1, 4 };
                return true;
            }
        }

        else if (name == "Vestal")
        {
            if (skillNumber == 1) // 1.9
            {
                rules = { 1, 4, 1, 2 };
                return true;
            }

            if (skillNumber == 2) // 1.10
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

// 1.11
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 1.12
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

// 1.13
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

// 1.14
std::string getRandomNarration(bool playerTurn)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    // 1.15
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

    // 1.16
    const int phraseCount = 8;

    // 1.17
    std::uniform_int_distribution<int> distribution(
        0,
        phraseCount - 1
    );

    int index = distribution(generator);

    // 1.18
    if (playerTurn)
    {
        return playerLines[index];
    }

    return enemyLines[index];
}

// 1.19
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
    // 1.20
    const std::string GREEN = "\033[32m";
    const std::string CRIMSON = "\033[38;5;88m";
    const std::string GOLD = "\033[38;5;180m";
    const std::string RESET = "\033[0m";

    const int panelWidth = 100;
    const int playerColumnWidth = 60;

    // 1.21
    auto printWrappedText =
        [](const std::string& text, int maximumWidth)
        {
            if (text.empty())
            {
                std::cout << "  \n";
                return;
            }

            std::size_t start = 0;

            while (start < text.length())
            {
                // 1.22
                std::size_t end = start + maximumWidth;

                if (end > text.length())
                {
                    end = text.length();
                }

                // 1.23
                if (end < text.length())
                {
                    std::size_t space =
                        text.rfind(' ', end);

                    if (space != std::string::npos &&
                        space > start)
                    {
                        end = space;
                    }
                }

                // 1.24
                std::cout
                    << "  "
                    << text.substr(start, end - start)
                    << "\n";

                // 1.25
                start = end;

                while (start < text.length() &&
                    text[start] == ' ')
                {
                    start++;
                }
            }
        };

    // 1.26
    auto printMessagePanel =
        [&](const std::string& title,
            const std::string& message,
            const std::string& color)
        {
            std::cout
                << color
                << std::string(panelWidth, '=')
                << RESET
                << "\n";

            std::cout
                << color
                << "  "
                << title
                << RESET
                << "\n";

            std::cout
                << color
                << std::string(panelWidth, '-')
                << RESET
                << "\n";

            printWrappedText(message, panelWidth - 4);

            std::cout
                << color
                << std::string(panelWidth, '=')
                << RESET
                << "\n";
        };

    // 1.27
    std::string displayedNarration = narratorMessage;

    if (displayedNarration.empty())
    {
        displayedNarration =
            "The cellar holds its breath.";
    }

    printMessagePanel(
        "NARRATOR",
        displayedNarration,
        GOLD
    );

    // 1.28
    std::cout
        << GREEN
        << std::left
        << std::setw(playerColumnWidth)
        << "PLAYER PARTY"
        << RESET;

    std::cout
        << CRIMSON
        << "ENEMY PARTY"
        << RESET
        << "\n";

    std::cout
        << std::string(panelWidth, '-')
        << "\n";

    // 1.29
    for (int i = 1; i <= 4; i++)
    {
        Character* player =
            playerParty.getMember(i);

        Character* enemy =
            enemyParty.getMember(i);

        // 1.30
        std::string playerLine;

        if (player != nullptr)
        {
            playerLine =
                std::to_string(i) +
                ". " +
                player->getName() +
                " | HP " +
                std::to_string(player->getHP()) +
                "/" +
                std::to_string(player->getMaxHP()) +
                " | Stress " +
                std::to_string(player->getStress()) +
                "/200 | Pos " +
                std::to_string(
                    playerParty.getPosition(*player)
                );
        }

        // 1.31
        std::string enemyLine;

        if (enemy != nullptr)
        {
            enemyLine =
                std::to_string(i) +
                ". " +
                enemy->getName() +
                " | HP " +
                std::to_string(enemy->getHP()) +
                "/" +
                std::to_string(enemy->getMaxHP()) +
                " | Pos " +
                std::to_string(
                    enemyParty.getPosition(*enemy)
                );
        }

        // 1.32
        std::cout
            << GREEN
            << std::left
            << std::setw(playerColumnWidth)
            << playerLine
            << RESET;

        if (enemy != nullptr)
        {
            std::cout
                << CRIMSON
                << enemyLine
                << RESET;
        }

        std::cout << "\n";
    }

    // 1.33
    std::cout
        << std::string(panelWidth, '-')
        << "\n\n";

    // 1.34
    std::string displayedAction = lastActionMessage;

    if (displayedAction.empty())
    {
        displayedAction = "The battle has yet to begin.";
    }

    printMessagePanel(
        "LAST ACTION",
        displayedAction,
        GOLD
    );

}

bool Battle::movePartyMember()
{
    // 1.35
    while (true)
    {
        clearScreen();
        printBattleState();

        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << "       MOVE PARTY MEMBER\n";
        std::cout << "====================================\n\n";

        playerParty.printFormation();

        std::cout << "\n0. Back\n";

        // 1.36
        int fromPosition = Input::getIntInRange(
            "\nSelect a position to move: ",
            0,
            4
        );

        if (fromPosition == 0)
        {
            return false;
        }

        // 1.37
        Character* movingCharacter =
            playerParty.getMember(fromPosition);

        // 1.38
        if (movingCharacter == nullptr)
        {
            std::cout << "There is no character in that position.\n";

            Input::waitForEnter();
            continue;
        }

        // 1.39
        if (!movingCharacter->isAlive())
        {
            std::cout << "A defeated character cannot be moved.\n";

            Input::waitForEnter();
            continue;
        }

        // 1.40
        clearScreen();
        printBattleState();

        std::cout << "\n";
        std::cout << "Moving "
            << movingCharacter->getName()
            << " from position "
            << fromPosition
            << ".\n";

        playerParty.printFormation();

        std::cout << "\n0. Cancel\n";

        int toPosition = Input::getIntInRange(
            "Choose a destination: ",
            0,
            4
        );

        if (toPosition == 0)
        {
            return false;
        }

        // 1.41
        if (fromPosition == toPosition)
        {
            std::cout
                << "That character is already in that position.\n";

            Input::waitForEnter();
            continue;
        }

        // 1.42
        Character* displacedCharacter =
            playerParty.getMember(toPosition);

        // 1.43
        bool moveSucceeded = playerParty.moveMember(
            fromPosition,
            toPosition
        );

        if (!moveSucceeded)
        {
            std::cout << "The formation could not be changed.\n";

            Input::waitForEnter();
            continue;
        }

        // 1.44
        if (displacedCharacter != nullptr)
        {
            lastActionMessage =
                movingCharacter->getName() +
                " moves from position " +
                std::to_string(fromPosition) +
                " to position " +
                std::to_string(toPosition) +
                ", swapping positions with " +
                displacedCharacter->getName() +
                ".";
        }
        else
        {
            lastActionMessage =
                movingCharacter->getName() +
                " moves from position " +
                std::to_string(fromPosition) +
                " to position " +
                std::to_string(toPosition) +
                ".";
        }

        // 1.45
        return true;
    }
}

// 1.46
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

// 1.47
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

// 1.48
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

// 1.49

bool Battle::attackMenu(Character& character)
{
    // 1.50
    PositionRules rules;

    if (!getAttackRules(character, rules))
    {
        // 1.51
        std::cout
            << "No basic attack rules exist for this character.\n";

        Input::waitForEnter();

        return false;
    }

    // 1.52
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

        // 1.53
        return false;
    }

    // 1.54
    Character* target = chooseEnemyTarget(
        character,
        rules.targetMin,
        rules.targetMax
    );

    if (target == nullptr)
    {
        // 1.55
        return false;
    }

    // 1.56
    int hpBefore = target->getHP();

    // 1.57
    character.Character::attack(*target);

    // 1.58
    int damageDealt = hpBefore - target->getHP();

    // 1.59
    lastActionMessage =
        character.getName() +
        " attacks " +
        target->getName() +
        " for " +
        std::to_string(damageDealt) +
        " damage.";

    // 1.60
    if (!target->isAlive())
    {
        lastActionMessage +=
            " " +
            target->getName() +
            " has been defeated!";
    }

    // 1.61
    return true;
}

// 1.62

void Battle::skillMenu(Character& character)
{
    // 1.63
    while (true)
    {
        clearScreen();
        printBattleState();

        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << character.getName() << " Skills\n";
        std::cout << "====================================\n";

        // 1.64
        character.printSkills();

        std::cout << "0. Back\n";

        // 1.65
        int choice = Input::getInt(
            "\nChoose a skill: "
        );

        if (choice == 0)
        {
            return;
        }

        // 1.66
        PositionRules rules;

        if (!getSkillRules(character, choice, rules))
        {
            std::cout << "Invalid skill.\n";

            Input::waitForEnter();

            continue;
        }

        // 1.67
        int userPosition =
            playerParty.getPosition(character);

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

            // 1.68
            continue;
        }

        // 1.69
        TargetType targetType =
            character.getSkillTargetType(choice);

        Character* target = nullptr;

        // 1.70
        if (targetType == TargetType::Enemy)
        {
            // 1.71
            target = chooseEnemyTarget(
                character,
                rules.targetMin,
                rules.targetMax
            );
        }
        else if (targetType == TargetType::Ally)
        {
            // 1.72
            target = chooseAllyTarget();
        }

        // 1.73
        if (target == nullptr)
        {
            continue;
        }

        // 1.74
        int hpBefore = target->getHP();

        // 1.75
        std::string skillName = "Skill";

        if (character.getName() == "Crusader")
        {
            if (choice == 1)
            {
                skillName = "Holy Strike";
            }
            else if (choice == 2)
            {
                skillName = "Heal Ally";
            }
        }
        else if (character.getName() == "Highwayman")
        {
            if (choice == 1)
            {
                skillName = "Pistol Shot";
            }
            else if (choice == 2)
            {
                skillName = "Melee Attack";
            }
        }
        else if (character.getName() == "Plague Doctor")
        {
            if (choice == 1)
            {
                skillName = "Plague Grenade";
            }
            else if (choice == 2)
            {
                skillName = "Heal Ally";
            }
        }
        else if (character.getName() == "Vestal")
        {
            if (choice == 1)
            {
                skillName = "Smite";
            }
            else if (choice == 2)
            {
                skillName = "Heal Ally";
            }
        }

        // 1.76
        character.useSkill(choice, *target);

        // 1.77
        int hpAfter = target->getHP();

        // 1.78
        if (targetType == TargetType::Enemy)
        {
            int damageDealt = hpBefore - hpAfter;

            lastActionMessage =
                character.getName() +
                " uses " +
                skillName +
                " on " +
                target->getName() +
                ", dealing " +
                std::to_string(damageDealt) +
                " damage.";

            // 1.79
            if (!target->isAlive())
            {
                lastActionMessage +=
                    " " +
                    target->getName() +
                    " has been defeated!";
            }
        }
        else if (targetType == TargetType::Ally)
        {
            int healingReceived = hpAfter - hpBefore;

            lastActionMessage =
                character.getName() +
                " uses " +
                skillName +
                " on " +
                target->getName() +
                ", restoring " +
                std::to_string(healingReceived) +
                " HP.";
        }

        // 1.80
        return;
    }
}

bool Battle::resolveStressReaction(Character& character)
{
    // 1.81
    if (character.getStress() <= 100)
    {
        return false;
    }

    // 1.82
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

    // 1.83
    std::uniform_int_distribution<int> reactionDistribution(
        1,
        3
    );

    int reaction = reactionDistribution(generator);

    // 1.84
    if (reaction == 1)
    {
        Character* targets[4];
        int targetCount = 0;

        // 1.85
        for (int i = 1; i <= 4; i++)
        {
            Character* enemy = enemyParty.getMember(i);

            if (enemy != nullptr && enemy->isAlive())
            {
                targets[targetCount] = enemy;
                targetCount++;
            }
        }

        // 1.86
        if (targetCount == 0)
        {
            return false;
        }

        // 1.87
        std::uniform_int_distribution<int> targetDistribution(
            0,
            targetCount - 1
        );

        Character* target =
            targets[targetDistribution(generator)];

        // 1.88
        int hpBefore = target->getHP();

        // 1.89
        character.Character::attack(*target);

        // 1.90
        int damageDealt = hpBefore - target->getHP();

        // 1.91
        lastActionMessage =
            character.getName() +
            " loses control and attacks " +
            target->getName() +
            " for " +
            std::to_string(damageDealt) +
            " damage!";

        narratorMessage =
            "Desperation overwhelms discipline.";

        // 1.92
        if (!target->isAlive())
        {
            lastActionMessage +=
                " " +
                target->getName() +
                " has been defeated!";
        }

        return true;
    }

    // 1.93
    if (reaction == 2)
    {
        lastActionMessage =
            character.getName() +
            " is overwhelmed by stress and refuses to act!";

        narratorMessage =
            "Fear steals the will to act.";

        return true;
    }

    // 1.94
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

    // 1.95
    if (allyCount == 0)
    {
        lastActionMessage =
            character.getName() +
            " is overwhelmed by stress and cannot act!";

        narratorMessage =
            "The mind collapses beneath the strain.";

        return true;
    }

    // 1.96
    std::uniform_int_distribution<int> allyDistribution(
        0,
        allyCount - 1
    );

    Character* target =
        allies[allyDistribution(generator)];

    // 1.97
    int hpBefore = target->getHP();

    // 1.98
    character.Character::attack(*target);

    int damageDealt = hpBefore - target->getHP();

    // 1.99
    int stressIncrease = randomStress(1, 4);
    target->addStress(stressIncrease);

    // 1.100
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

// 1.101

void Battle::playerTurn(Character& character)
{
    narratorMessage = getRandomNarration(true);

    // 1.102
    bool gainedStress = false;
    int stressIncrease = 0;

    if (stressOccurs())
    {
        stressIncrease = randomStress(1, 3);
        character.addStress(stressIncrease);
        gainedStress = true;
    }

    // 1.103
    if (resolveStressReaction(character))
    {
        clearScreen();
        printBattleState();

        Input::waitForEnter();

        // 1.104
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

        // 1.105
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

        // 1.106
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

        // 1.107
        if (choice == 1)
        {
            if (attackMenu(character))
            {
                return; // 1.108
            }

            continue; // 1.109
        }

        // 1.110
        if (choice == 2)
        {
            skillMenu(character);
            return;
        }

        // 1.111
        if (choice == 3)
        {
            if (movePartyMember())
            {
                return; // 1.112
            }

            continue; // 1.113
        }

        // 1.114
        if (choice == 4)
        {
            std::cout
                << character.getName()
                << " passes.\n";

            return;
        }
    }
}

// 1.115

void Battle::enemyTurn(Character& enemy)
{
    narratorMessage = getRandomNarration(false);
    // 1.116
    clearScreen();
    printBattleState();

    // 1.117
    if (!enemy.isAlive())
    {
        return;
    }

    // 1.118
    std::cout << "\n";
    std::cout << "------------------------------------\n";
    std::cout << enemy.getName() << "'s turn!\n";
    std::cout << "------------------------------------\n";

    // 1.119
    Character* targets[4];
    int targetCount = 0;

    for (int position = 1; position <= 4; position++)
    {
        Character* target =
            playerParty.getMember(position);

        // 1.120
        if (target != nullptr && target->isAlive())
        {
            targets[targetCount] = target;
            targetCount++;
        }
    }

    // 1.121
    if (targetCount == 0)
    {
        return;
    }

    // 1.122
    static std::random_device rd;
    static std::mt19937 generator(rd());

    // 1.123
    std::uniform_int_distribution<int> targetDistribution(
        0,
        targetCount - 1
    );

    Character* target =
        targets[targetDistribution(generator)];

    // 1.124
    int hpBefore = target->getHP();

    // 1.125
    enemy.attack(*target);

    // 1.126
    int damageDealt = hpBefore - target->getHP();

    // 1.127
    int stressIncrease = randomStress(1, 4);

    target->addStress(stressIncrease);

    // 1.128
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

    // 1.129
    if (!target->isAlive())
    {
        lastActionMessage +=
            " " +
            target->getName() +
            " has been defeated!";
    }

    // 1.130
    if (target->isAlive())
    {
        std::uniform_int_distribution<int> displacementChance(
            1,
            100
        );

        if (displacementChance(generator) <= 25)
        {
            // 1.131
            int fromPosition =
                playerParty.getPosition(*target);

            // 1.132
            std::uniform_int_distribution<int> positionDistribution(
                1,
                4
            );

            int toPosition;

            do
            {
                toPosition = positionDistribution(generator);
            } while (toPosition == fromPosition);

            // 1.133
            Character* displacedCharacter =
                playerParty.getMember(toPosition);

            // 1.134
            bool moved = playerParty.moveMember(
                fromPosition,
                toPosition
            );

            // 1.135
            if (moved)
            {
                if (displacedCharacter != nullptr)
                {
                    // 1.136
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
                    // 1.137
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

    // 1.138
}

// 1.139

void Battle::start()
{
    // 1.140
    clearScreen();

    std::cout << "====================================\n";
    std::cout << "          BATTLE START!\n";
    std::cout << "====================================\n";

    printBattleState();

    // 1.141
    while (playerPartyAlive() && enemyPartyAlive())
    {
        // 1.142
        Character* playerTurnOrder[4] = {
            nullptr, nullptr, nullptr, nullptr
        };

        Character* enemyTurnOrder[4] = {
            nullptr, nullptr, nullptr, nullptr
        };

        for (int position = 1; position <= 4; position++)
        {
            Character* player =
                playerParty.getMember(position);

            Character* enemy =
                enemyParty.getMember(position);

            if (player != nullptr && player->isAlive())
            {
                playerTurnOrder[position - 1] = player;
            }

            if (enemy != nullptr && enemy->isAlive())
            {
                enemyTurnOrder[position - 1] = enemy;
            }
        }

        // 1.143
        for (int i = 0; i < 2; i++)
        {
            if (!enemyPartyAlive())
            {
                break;
            }

            Character* character = playerTurnOrder[i];

            if (character != nullptr && character->isAlive())
            {
                playerTurn(*character);
            }
        }

        // 1.144
        if (!playerPartyAlive() || !enemyPartyAlive())
        {
            break;
        }

        // 1.145
        for (int i = 0; i < 2; i++)
        {
            if (!playerPartyAlive())
            {
                break;
            }

            Character* enemy = enemyTurnOrder[i];

            if (enemy != nullptr && enemy->isAlive())
            {
                enemyTurn(*enemy);
            }
        }

        // 1.146
        if (!playerPartyAlive() || !enemyPartyAlive())
        {
            break;
        }

        // 1.147
        for (int i = 2; i < 4; i++)
        {
            if (!enemyPartyAlive())
            {
                break;
            }

            Character* character = playerTurnOrder[i];

            if (character != nullptr && character->isAlive())
            {
                playerTurn(*character);
            }
        }

        // 1.148
        if (!playerPartyAlive() || !enemyPartyAlive())
        {
            break;
        }

        // 1.149
        for (int i = 2; i < 4; i++)
        {
            if (!playerPartyAlive())
            {
                break;
            }

            Character* enemy = enemyTurnOrder[i];

            if (enemy != nullptr && enemy->isAlive())
            {
                enemyTurn(*enemy);
            }
        }

        // 1.150
        clearScreen();
        printBattleState();
    }

    // 1.151
    clearScreen();
    printBattleState();

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

// 1.152
Party& Battle::getPlayerParty()
{
    return playerParty;
}

Party& Battle::getEnemyParty()
{
    return enemyParty;
}
