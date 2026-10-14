
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



// Selects one random line from a list of narrator phrases.
std::string getRandomNarration(bool playerTurn)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    // Different narration pools for heroes and enemies.
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

    // Count the phrases available in each array.
    const int phraseCount = 8;

    // Pick a random phrase index.
    std::uniform_int_distribution<int> distribution(
        0,
        phraseCount - 1
    );

    int index = distribution(generator);

    // Return a phrase appropriate to the current side.
    if (playerTurn)
    {
        return playerLines[index];
    }

    return enemyLines[index];
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
    // ==================================================
    // COLOR SETTINGS
    // ==================================================
    //
    // These ANSI escape codes control the text colors
    // displayed in the console.
    //
    // GREEN   = Player party
    // CRIMSON = Enemy party
    // RESET   = Restores the console's default color
    //
    // Keeping these in local constants makes the
    // display code easier to read and maintain.

    const std::string GREEN = "\033[32m";
    const std::string CRIMSON = "\033[38;5;88m";
    const std::string RESET = "\033[0m";


    // ==================================================
    // COLUMN CONFIGURATION
    // ==================================================
    //
    // COLUMN_WIDTH determines how far apart the two
    // party columns appear on the screen.
    //
    // printColumnGap() uses the visible text length to
    // calculate the spacing between the columns.
    //
    // This keeps the enemy party aligned even when
    // character names have different lengths.

    const int columnWidth = 45;


    // ==================================================
    // BATTLE HEADER
    // ==================================================
    //
    // Print the top border and the title.
    //
    // The border gives the battle screen a clear
    // beginning and separates it from other messages.

    std::cout << "\n";

    std::cout
        << "================================================================================\n";

    std::cout
        << "                                  BATTLE\n";

    std::cout
        << "================================================================================\n\n";


    // ==================================================
    // PARTY HEADERS
    // ==================================================
    //
    // Print PLAYER PARTY on the left and ENEMY PARTY
    // on the right.
    //
    // std::left and std::setw() align the player header
    // with the player column.
    //
    // ANSI color codes are applied around the displayed
    // text so the two sides are easy to distinguish.

    std::cout
        << GREEN
        << std::left
        << std::setw(columnWidth)
        << "PLAYER PARTY"
        << RESET;

    std::cout
        << CRIMSON
        << "ENEMY PARTY"
        << RESET;

    std::cout << "\n";

    std::cout
        << "--------------------------------------------------------------------------------\n";


    // ==================================================
    // LAST ACTION DISPLAY
    // ==================================================
    //
    // lastActionMessage is a member variable in Battle.
    //
    // Other battle functions save messages here after
    // attacks, skills, and enemy actions.
    //
    // Because printBattleState() is called again after
    // the console clears, displaying this variable lets
    // the player see what happened during the last action.
    //
    // If the string is empty, no action message is shown.
    //
    // IMPORTANT:
    // This section only DISPLAYs the message.
    // attackMenu(), skillMenu(), and enemyTurn() are
    // responsible for creating and saving the message.

    if (!lastActionMessage.empty())
    {
        std::cout << "\n";

        std::cout << "LAST ACTION\n";

        std::cout << lastActionMessage << "\n";

        std::cout << "\n";

        // Add a divider below the action message so the
        // message is visually separated from the parties.
        std::cout
            << "--------------------------------------------------------------------------------\n";
    }
    const std::string GOLD = "\033[38;5;180m";
    

    std::cout << "\n";
    std::cout << GOLD;
    std::cout << "===============================================\n";
    std::cout << "                  NARRATOR\n";
    std::cout << "===============================================\n";
    std::cout << "\"" << narratorMessage << "\"\n";
    std::cout << "===============================================\n";
    std::cout << RESET << "\n";


    // ==================================================
    // CHARACTER FORMATION DISPLAY
    // ==================================================
    //
    // Each party has four possible formation positions.
    //
    // We iterate from position 1 through position 4.
    //
    // getMember(i) retrieves the character occupying
    // that slot. If a slot is empty, it returns nullptr.
    //
    // Empty enemy slots are intentionally not printed.
    // This means a battle with two enemies displays only
    // those two enemies rather than printing "Empty".

    for (int i = 1; i <= 4; i++)
    {
        // Retrieve the current player and enemy from
        // their respective formation positions.
        Character* player = playerParty.getMember(i);
        Character* enemy = enemyParty.getMember(i);


        // ==================================================
        // CHARACTER NAMES
        // ==================================================
        //
        // Start with empty strings.
        //
        // We only populate a string if a character
        // actually occupies that formation position.
        //
        // The position number is included before each
        // name to make movement and targeting easier
        // to understand.

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


        // ==================================================
        // PRINT PLAYER NAME
        // ==================================================
        //
        // Player names are displayed in green.
        //
        // We do not print anything here if the player
        // position is empty.

        if (player != nullptr)
        {
            std::cout
                << GREEN
                << playerName
                << RESET;
        }


        // ==================================================
        // ALIGN THE ENEMY COLUMN
        // ==================================================
        //
        // printColumnGap() calculates the remaining
        // spacing needed to position the enemy column.
        //
        // It uses the player name's length so shorter
        // names receive more spaces than longer names.
        //
        // If the player slot is empty, the empty string
        // is passed and the spacing remains consistent.

        printColumnGap(playerName);


        // ==================================================
        // PRINT ENEMY NAME
        // ==================================================
        //
        // Only occupied enemy positions are displayed.
        //
        // If enemy is nullptr, neither the name nor
        // an "Empty" placeholder is printed.

        if (enemy != nullptr)
        {
            std::cout
                << CRIMSON
                << enemyName
                << RESET;
        }

        std::cout << "\n";


        // ==================================================
        // PLAYER HP AND STRESS
        // ==================================================
        //
        // Display the player's current HP, maximum HP,
        // and accumulated stress.
        //
        // Stress is shown out of 200, matching the
        // limits used by your existing Character system.
        //
        // These values are read directly from Character,
        // so changes from combat or room events are
        // reflected whenever this screen is redrawn.

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

            std::cout
                << GREEN
                << playerStats
                << RESET;
        }


        // Align the enemy's HP information with the
        // enemy column, just like the character names.

        printColumnGap(playerStats);


        // ==================================================
        // ENEMY HP
        // ==================================================
        //
        // Enemies display their current HP and maximum HP.
        //
        // We intentionally do not print stress or an
        // HP placeholder for an empty enemy position.

        if (enemy != nullptr)
        {
            std::string enemyStats =
                "   HP: " +
                std::to_string(enemy->getHP()) +
                "/" +
                std::to_string(enemy->getMaxHP());

            std::cout
                << CRIMSON
                << enemyStats
                << RESET;
        }

        std::cout << "\n";


        // ==================================================
        // PLAYER POSITION
        // ==================================================
        //
        // Retrieve the player's position through Party,
        // which delegates to Formation.
        //
        // This means the display uses the formation's
        // current position after a character moves.
        //
        // We do not print a position for an empty slot.

        std::string playerPosition;

        if (player != nullptr)
        {
            playerPosition =
                "   Position: " +
                std::to_string(
                    playerParty.getPosition(*player)
                );

            std::cout
                << GREEN
                << playerPosition
                << RESET;
        }


        // Keep the enemy position aligned with the
        // corresponding player information.

        printColumnGap(playerPosition);


        // ==================================================
        // ENEMY POSITION
        // ==================================================
        //
        // Retrieve the enemy's position through its Party
        // and Formation in the same way as the players.
        //
        // If there is no enemy in this slot, the enemy
        // position is omitted entirely.

        if (enemy != nullptr)
        {
            std::string enemyPosition =
                "   Position: " +
                std::to_string(
                    enemyParty.getPosition(*enemy)
                );

            std::cout
                << CRIMSON
                << enemyPosition
                << RESET;
        }

        // Leave a blank line between formation positions
        // so each character's information is easy to scan.

        std::cout << "\n\n";
    }


    // ==================================================
    // BATTLE FOOTER
    // ==================================================
    //
    // Finish the screen with the bottom border.
    //
    // The complete battle state can now be redrawn
    // without losing the last recorded action message.

    std::cout
        << "================================================================================\n";
}



bool Battle::movePartyMember()
{
    // ==================================================
    // 1. DISPLAY THE CURRENT FORMATION
    // ==================================================
    //
    // Clear the console and redraw the battle state so
    // the player can see the current positions.
    //
    // Party::printFormation() displays positions 1-4.
    // Party delegates formation management to Formation.

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


        // ==================================================
        // 2. SELECT THE CHARACTER TO MOVE
        // ==================================================
        //
        // The player selects a formation position, not
        // a character name.
        //
        // Position 0 cancels the operation and returns
        // to the character's action menu.

        int fromPosition = Input::getIntInRange(
            "\nSelect a position to move: ",
            0,
            4
        );

        if (fromPosition == 0)
        {
            return false;
        }


        // Retrieve the character occupying that position.

        Character* movingCharacter =
            playerParty.getMember(fromPosition);


        // A position must contain a character.

        if (movingCharacter == nullptr)
        {
            std::cout << "There is no character in that position.\n";

            Input::waitForEnter();
            continue;
        }


        // Only living characters can be selected to move.

        if (!movingCharacter->isAlive())
        {
            std::cout << "A defeated character cannot be moved.\n";

            Input::waitForEnter();
            continue;
        }


        // ==================================================
        // 3. SELECT THE DESTINATION
        // ==================================================
        //
        // Show the current formation again so the player
        // can decide where the selected character should go.
        //
        // An occupied destination causes a swap.
        // An empty destination receives the character.

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


        // Moving to the same position changes nothing.
        // Let the player choose again.

        if (fromPosition == toPosition)
        {
            std::cout
                << "That character is already in that position.\n";

            Input::waitForEnter();
            continue;
        }


        // ==================================================
        // 4. RECORD WHO OCCUPIES THE DESTINATION
        // ==================================================
        //
        // Save this before moving anyone.
        //
        // If a character already occupies the destination,
        // Formation::moveMember() will swap the two.
        //
        // We retain their name so the action message can
        // explain the resulting movement.

        Character* displacedCharacter =
            playerParty.getMember(toPosition);


        // ==================================================
        // 5. MOVE THE CHARACTER THROUGH FORMATION
        // ==================================================
        //
        // Do not edit positions directly here.
        //
        // Party::moveMember() delegates to Formation,
        // which updates the formation slots and synchronizes
        // the affected characters' current positions.
        //
        // This keeps your battle range checks working.

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


        // ==================================================
        // 6. SAVE THE LAST-ACTION MESSAGE
        // ==================================================
        //
        // printBattleState() displays lastActionMessage
        // whenever the battle screen is redrawn.
        //
        // Describe the move differently depending on
        // whether the destination was occupied.

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


        // ==================================================
        // 7. FINISH THE MOVEMENT ACTION
        // ==================================================
        //
        // Return true to indicate a successful move.
        //
        // playerTurn() will then end the current hero's
        // turn and let the battle continue.

        return true;
    }
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
    // ==================================================
    // 1. RETRIEVE THE CHARACTER'S ATTACK RULES
    // ==================================================
    //
    // PositionRules contains four values:
    //
    // userMin   = minimum position the attacker can use
    // userMax   = maximum position the attacker can use
    // targetMin = minimum enemy position that can be hit
    // targetMax = maximum enemy position that can be hit
    //
    // getAttackRules() retrieves the correct rules for
    // the current character.
    //
    // Example: Highwayman
    // { 1, 3, 2, 4 }
    // Can attack from positions 1-3 and target enemies
    // in positions 2-4.

    PositionRules rules;

    if (!getAttackRules(character, rules))
    {
        // No attack rules exist for this character.
        // Do not execute the attack.

        std::cout
            << "No basic attack rules exist for this character.\n";

        Input::waitForEnter();

        return false;
    }


    // ==================================================
    // 2. CHECK THE ATTACKER'S FORMATION POSITION
    // ==================================================
    //
    // Retrieve the character's current position through
    // playerParty, which delegates to Formation.
    //
    // This is important because Formation is responsible
    // for managing the party's current arrangement.
    //
    // We check the attacker's position before allowing
    // them to choose an enemy target.

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

        // Return false because no attack occurred.
        // This allows playerTurn() to let the player
        // choose another action instead of losing a turn.

        return false;
    }


    // ==================================================
    // 3. SELECT A VALID ENEMY TARGET
    // ==================================================
    //
    // Only enemies within the basic attack's target
    // range are offered to the player.
    //
    // chooseEnemyTarget() also handles:
    // - Dead enemies
    // - Invalid target positions
    // - The player's Back option
    //
    // If the player backs out, no attack occurs.

    Character* target = chooseEnemyTarget(
        character,
        rules.targetMin,
        rules.targetMax
    );

    if (target == nullptr)
    {
        // No target was selected, so do not consume
        // the player's turn or replace the last action.

        return false;
    }


    // ==================================================
    // 4. RECORD THE TARGET'S HP BEFORE THE ATTACK
    // ==================================================
    //
    // Save the target's current HP before applying damage.
    //
    // Comparing HP before and after the attack allows
    // us to determine how much HP the target actually lost.
    //
    // This is better than relying on the attack's random
    // damage roll because the target might have less HP
    // remaining than the damage dealt.

    int hpBefore = target->getHP();


    // ==================================================
    // 5. EXECUTE THE BASIC ATTACK
    // ==================================================
    //
    // Call Character's base attack implementation directly.
    //
    // The Battle system has already validated the
    // attacker's position and selected a valid target.
    //
    // Calling the base implementation directly prevents
    // an old character-specific attack override from
    // applying a second set of position restrictions.

    character.Character::attack(*target);


    // ==================================================
    // 6. CALCULATE THE ACTUAL DAMAGE DEALT
    // ==================================================
    //
    // Example:
    // HP before attack = 20
    // HP after attack  = 13
    // Actual damage    = 7
    //
    // This reports the amount of HP actually lost.

    int damageDealt = hpBefore - target->getHP();


    // ==================================================
    // 7. SAVE THE ACTION MESSAGE
    // ==================================================
    //
    // lastActionMessage is stored in Battle.h as a
    // std::string member variable.
    //
    // printBattleState() displays this message after
    // the console clears and the battle screen redraws.
    //
    // We save the result here, after the attack has
    // actually executed, so the message reflects what
    // happened rather than what was intended.

    lastActionMessage =
        character.getName() +
        " attacks " +
        target->getName() +
        " for " +
        std::to_string(damageDealt) +
        " damage.";


    // ==================================================
    // 8. CHECK WHETHER THE TARGET WAS DEFEATED
    // ==================================================
    //
    // If the attack reduced the target's HP to zero,
    // append a defeat message to the existing action.
    //
    // The message is displayed together with the
    // damage result on the next battle-screen redraw.

    if (!target->isAlive())
    {
        lastActionMessage +=
            " " +
            target->getName() +
            " has been defeated!";
    }


    // ==================================================
    // 9. REPORT THAT THE ACTION SUCCEEDED
    // ==================================================
    //
    // Returning true tells playerTurn() that an attack
    // was completed and the character's turn is over.
    //
    // This differs from returning false when the player
    // cannot attack or backs out of target selection.

    return true;
}


// --------------------------------------------------
// Skill menu
// --------------------------------------------------


void Battle::skillMenu(Character& character)
{
    // ==================================================
    // 1. DISPLAY THE SKILL MENU
    // ==================================================
    //
    // Keep showing the skill menu until:
    // - The player successfully uses a skill.
    // - The player chooses Back.
    //
    // The battle screen is redrawn each time the menu
    // opens. printBattleState() also displays the most
    // recent action saved in lastActionMessage.

    while (true)
    {
        clearScreen();
        printBattleState();

        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << character.getName() << " Skills\n";
        std::cout << "====================================\n";

        // Display the skills available to this character.
        character.printSkills();

        std::cout << "0. Back\n";


        // ==================================================
        // 2. GET THE PLAYER'S CHOICE
        // ==================================================
        //
        // Input::getInt() handles invalid integer input.
        //
        // Choosing 0 returns to the character's action menu.
        // No skill is used and the previous action message
        // remains unchanged.

        int choice = Input::getInt(
            "\nChoose a skill: "
        );

        if (choice == 0)
        {
            return;
        }


        // ==================================================
        // 3. RETRIEVE THE SKILL'S POSITION RULES
        // ==================================================
        //
        // PositionRules contains:
        //
        // userMin   = minimum position the character can use
        // userMax   = maximum position the character can use
        // targetMin = minimum target position
        // targetMax = maximum target position
        //
        // getSkillRules() determines these rules based on
        // the character name and the selected skill number.
        //
        // If there are no rules for this skill, the choice
        // is invalid and the player remains in this menu.

        PositionRules rules;

        if (!getSkillRules(character, choice, rules))
        {
            std::cout << "Invalid skill.\n";

            Input::waitForEnter();

            continue;
        }


        // ==================================================
        // 4. VALIDATE THE USER'S FORMATION POSITION
        // ==================================================
        //
        // Retrieve the character's position from playerParty.
        // Party delegates this request to Formation.
        //
        // This ensures skill restrictions use the current
        // formation rather than relying on old range checks
        // inside individual character classes.

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

            // The skill was not used.
            // Allow the player to select a different skill.

            continue;
        }


        // ==================================================
        // 5. DETERMINE THE REQUIRED TARGET TYPE
        // ==================================================
        //
        // Each character identifies whether a skill targets
        // an enemy or an ally.
        //
        // Battle uses this information to open the
        // appropriate target-selection menu.

        TargetType targetType =
            character.getSkillTargetType(choice);

        Character* target = nullptr;


        // ==================================================
        // 6. SELECT A VALID TARGET
        // ==================================================

        if (targetType == TargetType::Enemy)
        {
            // Only show enemies within the skill's
            // permitted target-position range.

            target = chooseEnemyTarget(
                character,
                rules.targetMin,
                rules.targetMax
            );
        }
        else if (targetType == TargetType::Ally)
        {
            // Open the ally-selection menu.
            target = chooseAllyTarget();
        }

        // If the player backed out of target selection,
        // do not execute the skill or change the action log.

        if (target == nullptr)
        {
            continue;
        }


        // ==================================================
        // 7. RECORD THE TARGET'S HP BEFORE THE SKILL
        // ==================================================
        //
        // Store the target's current HP so we can calculate
        // what actually happened after the skill executes.
        //
        // For damaging skills, this measures actual HP lost.
        // For healing skills, this measures actual HP gained.
        //
        // For example:
        // Before: 12 HP
        // After:   5 HP
        // Damage:  7 HP

        int hpBefore = target->getHP();


        // ==================================================
        // 8. DETERMINE THE SKILL NAME
        // ==================================================
        //
        // Save a readable skill name for the action message.
        //
        // These names correspond to the skills currently
        // displayed by your four playable characters.
        //
        // The skill number is only interpreted after
        // getSkillRules() confirms that it is valid.

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


        // ==================================================
        // 9. EXECUTE THE SKILL
        // ==================================================
        //
        // At this point:
        // - The skill number has been validated.
        // - The user's position has been validated.
        // - A valid target has been selected.
        //
        // The character class now executes the actual
        // damage, healing, or other ability effect.

        character.useSkill(choice, *target);


        // ==================================================
        // 10. CALCULATE THE RESULT
        // ==================================================
        //
        // Compare HP before and after the skill.
        //
        // Enemy target:
        //     HP lost = damage dealt.
        //
        // Ally target:
        //     HP gained = healing received.
        //
        // Measuring actual HP changes means the action
        // message won't claim a hero recovered 8 HP if
        // that hero only needed 3 HP to reach full health.

        int hpAfter = target->getHP();


        // ==================================================
        // 11. SAVE THE LAST-ACTION MESSAGE
        // ==================================================
        //
        // printBattleState() displays this string whenever
        // the battle screen is redrawn.
        //
        // Saving the message here ensures it describes
        // the skill that has just executed.
        //
        // This message replaces the previous action
        // instead of allowing the console to fill up.

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


            // Append a defeat message if the attack
            // reduced the enemy's HP to zero.

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


        // ==================================================
        // 12. FINISH THE TURN
        // ==================================================
        //
        // The skill has successfully executed.
        // Return to playerTurn(), which ends this character's
        // turn and allows the battle loop to continue.

        return;
    }
}


bool Battle::resolveStressReaction(Character& character)
{
    // ==================================================
    // 1. CHECK THE STRESS THRESHOLD
    // ==================================================
    //
    // A hero must have MORE than 100 stress to be affected.
    // At 100 or below, their turn proceeds normally.

    if (character.getStress() <= 100)
    {
        return false;
    }


    // ==================================================
    // 2. ROLL FOR A STRESS REACTION
    // ==================================================
    //
    // There is a 30% chance of a reaction each turn
    // while the hero's stress remains above 100.
    //
    // A failed roll means the hero can act normally.

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


    // ==================================================
    // 3. CHOOSE THE REACTION
    // ==================================================
    //
    // Each of the three reactions currently has an equal
    // chance of occurring once a reaction is triggered.
    //
    // 1 = Attack a random enemy
    // 2 = Refuse the turn
    // 3 = Attack a random ally

    std::uniform_int_distribution<int> reactionDistribution(
        1,
        3
    );

    int reaction = reactionDistribution(generator);


    // ==================================================
    // REACTION 1: ATTACK A RANDOM ENEMY
    // ==================================================
    //
    // The stressed hero loses control and attacks a
    // randomly selected living enemy.
    //
    // Because this is an involuntary action, it uses
    // Character::attack() directly rather than the
    // hero's normal skill-selection and range checks.

    if (reaction == 1)
    {
        Character* targets[4];
        int targetCount = 0;

        // Collect all living enemies.
        for (int i = 1; i <= 4; i++)
        {
            Character* enemy = enemyParty.getMember(i);

            if (enemy != nullptr && enemy->isAlive())
            {
                targets[targetCount] = enemy;
                targetCount++;
            }
        }

        // Safety check in case there are no living enemies.
        if (targetCount == 0)
        {
            return false;
        }

        // Select a random enemy.
        std::uniform_int_distribution<int> targetDistribution(
            0,
            targetCount - 1
        );

        Character* target =
            targets[targetDistribution(generator)];

        // Record the target's HP before attacking.
        int hpBefore = target->getHP();

        // Execute the involuntary attack.
        character.Character::attack(*target);

        // Calculate the actual HP lost.
        int damageDealt = hpBefore - target->getHP();

        // Store the result for the battle screen.
        lastActionMessage =
            character.getName() +
            " loses control and attacks " +
            target->getName() +
            " for " +
            std::to_string(damageDealt) +
            " damage!";

        narratorMessage =
            "Desperation overwhelms discipline.";

        // Record the enemy's defeat if applicable.
        if (!target->isAlive())
        {
            lastActionMessage +=
                " " +
                target->getName() +
                " has been defeated!";
        }

        return true;
    }


    // ==================================================
    // REACTION 2: REFUSE TO ACT
    // ==================================================
    //
    // The hero is overwhelmed and refuses to take
    // their normal action this turn.
    //
    // No attack or skill is executed.

    if (reaction == 2)
    {
        lastActionMessage =
            character.getName() +
            " is overwhelmed by stress and refuses to act!";

        narratorMessage =
            "Fear steals the will to act.";

        return true;
    }


    // ==================================================
    // REACTION 3: ATTACK A RANDOM ALLY
    // ==================================================
    //
    // The stressed hero loses control and attacks
    // another living member of the player party.
    //
    // The acting hero cannot target themselves.
    // Normal position restrictions are ignored for
    // this involuntary attack.

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

    // If there is nobody else alive, the hero cannot
    // attack an ally, so the reaction becomes a refusal.
    if (allyCount == 0)
    {
        lastActionMessage =
            character.getName() +
            " is overwhelmed by stress and cannot act!";

        narratorMessage =
            "The mind collapses beneath the strain.";

        return true;
    }

    // Choose a random living ally.
    std::uniform_int_distribution<int> allyDistribution(
        0,
        allyCount - 1
    );

    Character* target =
        allies[allyDistribution(generator)];

    // Record the ally's HP before the attack.
    int hpBefore = target->getHP();

    // Execute the involuntary friendly-fire attack.
    character.Character::attack(*target);

    int damageDealt = hpBefore - target->getHP();

    // Being attacked by an ally also causes stress.
    int stressIncrease = randomStress(1, 4);
    target->addStress(stressIncrease);

    // Save the result for the battle display.
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



// --------------------------------------------------
// Player turn
// --------------------------------------------------


void Battle::playerTurn(Character& character)
{
    narratorMessage = getRandomNarration(true);

    // Roll for stress once at the start of the turn.
    // Opening menus or redrawing the screen won't reroll it.
    bool gainedStress = false;
    int stressIncrease = 0;

    if (stressOccurs())
    {
        stressIncrease = randomStress(1, 3);
        character.addStress(stressIncrease);
        gainedStress = true;
    }

    // Check whether stress forces an involuntary action.
    // This happens once per turn, not on each menu redraw.
    if (resolveStressReaction(character))
    {
        clearScreen();
        printBattleState();

        Input::waitForEnter();

        // The reaction consumed the hero's turn.
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

        // Display the stress change for this turn.
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

        // Available actions.
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

        // ------------------------------------------
        // OPTION 1: ATTACK
        // ------------------------------------------
        if (choice == 1)
        {
            if (attackMenu(character))
            {
                return; // Successful attack consumes the turn.
            }

            continue; // Back or invalid action: choose again.
        }

        // ------------------------------------------
        // OPTION 2: SKILLS
        // ------------------------------------------
        if (choice == 2)
        {
            skillMenu(character);
            return;
        }

        // ------------------------------------------
        // OPTION 3: MOVE PARTY MEMBER
        // ------------------------------------------
        if (choice == 3)
        {
            if (movePartyMember())
            {
                return; // Successful movement consumes the turn.
            }

            continue; // Cancelled movement: choose again.
        }

        // ------------------------------------------
        // OPTION 4: PASS
        // ------------------------------------------
        if (choice == 4)
        {
            std::cout
                << character.getName()
                << " passes.\n";

            return;
        }
    }
}

// --------------------------------------------------
// Enemy turn
// --------------------------------------------------


void Battle::enemyTurn(Character& enemy)
{
    narratorMessage = getRandomNarration(false);
    // ==================================================
    // 1. PREPARE THE BATTLE SCREEN
    // ==================================================
    //
    // Clear the previous console output and redraw the
    // current battle state before the enemy takes action.
    //
    // This lets the player see the current HP values and
    // formation positions before the enemy attacks.

    clearScreen();
    printBattleState();


    // ==================================================
    // 2. CHECK WHETHER THE ENEMY CAN ACT
    // ==================================================
    //
    // Defeated enemies must not take turns.
    //
    // This safety check prevents an enemy that has already
    // been defeated from attacking.

    if (!enemy.isAlive())
    {
        return;
    }


    // ==================================================
    // 3. DISPLAY THE ENEMY'S TURN
    // ==================================================
    //
    // Print a small heading so the player knows which
    // enemy is about to perform its action.

    std::cout << "\n";
    std::cout << "------------------------------------\n";
    std::cout << enemy.getName() << "'s turn!\n";
    std::cout << "------------------------------------\n";


    // ==================================================
    // 4. COLLECT LIVING PLAYER CHARACTERS
    // ==================================================
    //
    // Build an array containing only living members of
    // the player's party.
    //
    // The enemy will randomly choose one of these heroes.
    //
    // We store Character pointers because the party
    // already owns and manages the actual characters.
    // We do not create copies of the characters here.

    Character* targets[4];
    int targetCount = 0;

    for (int position = 1; position <= 4; position++)
    {
        Character* target =
            playerParty.getMember(position);

        // Ignore empty slots and defeated characters.
        if (target != nullptr && target->isAlive())
        {
            targets[targetCount] = target;
            targetCount++;
        }
    }


    // ==================================================
    // 5. CHECK WHETHER ANY HEROES REMAIN
    // ==================================================
    //
    // If no living targets were found, the enemy has
    // nobody to attack.
    //
    // Return immediately to avoid attempting to select
    // a target from an empty array.

    if (targetCount == 0)
    {
        return;
    }


    // ==================================================
    // 6. INITIALIZE THE RANDOM GENERATOR
    // ==================================================
    //
    // The random generator is reused for:
    // - Selecting an enemy's target.
    // - Determining whether repositioning happens.
    // - Choosing the destination position.
    //
    // The static declarations ensure the generator is
    // initialized once and reused on later enemy turns.

    static std::random_device rd;
    static std::mt19937 generator(rd());


    // ==================================================
    // 7. SELECT A RANDOM PLAYER TARGET
    // ==================================================
    //
    // Choose a random index from the array of living
    // heroes.
    //
    // Because the array contains only living characters,
    // the enemy cannot deliberately select an empty slot
    // or a defeated hero.

    std::uniform_int_distribution<int> targetDistribution(
        0,
        targetCount - 1
    );

    Character* target =
        targets[targetDistribution(generator)];


    // ==================================================
    // 8. RECORD THE TARGET'S HP BEFORE THE ATTACK
    // ==================================================
    //
    // Save the target's current HP before damage occurs.
    //
    // Comparing HP before and after the attack lets us
    // calculate the amount of HP actually lost.

    int hpBefore = target->getHP();


    // ==================================================
    // 9. EXECUTE THE ENEMY ATTACK
    // ==================================================
    //
    // The enemy performs its normal attack against the
    // selected player character.
    //
    // Character::attack() currently handles the damage
    // calculation and applies that damage to the target.

    enemy.attack(*target);


    // ==================================================
    // 10. CALCULATE THE ACTUAL DAMAGE
    // ==================================================
    //
    // Example:
    // HP before attack = 26
    // HP after attack  = 21
    // Actual HP lost   = 5
    //
    // This reports the actual HP reduction, rather than
    // assuming the target always loses the full damage
    // rolled by the attack.

    int damageDealt = hpBefore - target->getHP();


    // ==================================================
    // 11. APPLY STRESS TO THE PLAYER TARGET
    // ==================================================
    //
    // Only the player party gains stress when attacked.
    //
    // The target receives between 1 and 4 stress.
    // randomStress() is your existing random stress helper.
    //
    // Your Character::addStress() method handles the
    // stress limit, so this function does not need to
    // clamp the value itself.

    int stressIncrease = randomStress(1, 4);

    target->addStress(stressIncrease);


    // ==================================================
    // 12. CREATE THE INITIAL ACTION MESSAGE
    // ==================================================
    //
    // lastActionMessage is displayed by
    // printBattleState() after the screen redraws.
    //
    // Store the attack result and stress increase now.
    // If the target is subsequently repositioned, we
    // append that information to this same message.

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


    // ==================================================
    // 13. CHECK WHETHER THE TARGET WAS DEFEATED
    // ==================================================
    //
    // A defeated hero cannot be repositioned by this
    // effect.
    //
    // Record their defeat in the action message so the
    // result remains visible after the next redraw.

    if (!target->isAlive())
    {
        lastActionMessage +=
            " " +
            target->getName() +
            " has been defeated!";
    }


    // ==================================================
    // 14. ROLL FOR RANDOM REPOSITIONING
    // ==================================================
    //
    // Only a surviving hero can be repositioned.
    //
    // A roll from 1 to 100 gives a 25% chance when the
    // result is 25 or lower.
    //
    // The repositioning happens after the damage and
    // stress are applied.

    if (target->isAlive())
    {
        std::uniform_int_distribution<int> displacementChance(
            1,
            100
        );

        if (displacementChance(generator) <= 25)
        {
            // ==========================================
            // 15. GET THE TARGET'S CURRENT POSITION
            // ==========================================
            //
            // Retrieve the current position through Party.
            // Party delegates position lookup to Formation.
            //
            // We do not directly edit Character's position
            // field because Formation must stay synchronized.

            int fromPosition =
                playerParty.getPosition(*target);


            // ==========================================
            // 16. SELECT A DIFFERENT DESTINATION
            // ==========================================
            //
            // Randomly select a destination from positions
            // 1 through 4.
            //
            // The loop rejects the current position so the
            // character is guaranteed to change positions.

            std::uniform_int_distribution<int> positionDistribution(
                1,
                4
            );

            int toPosition;

            do
            {
                toPosition = positionDistribution(generator);
            } while (toPosition == fromPosition);


            // ==========================================
            // 17. CHECK THE DESTINATION OCCUPANT
            // ==========================================
            //
            // Formation::moveMember() swaps characters when
            // the destination is occupied.
            //
            // Save the destination occupant before moving
            // anyone so we can describe the result later.

            Character* displacedCharacter =
                playerParty.getMember(toPosition);


            // ==========================================
            // 18. MOVE THE CHARACTER THROUGH FORMATION
            // ==========================================
            //
            // Request the movement through Party instead
            // of changing the character's position directly.
            //
            // This updates the formation slots and keeps
            // position-dependent attack and skill rules
            // working with the new arrangement.

            bool moved = playerParty.moveMember(
                fromPosition,
                toPosition
            );


            // ==========================================
            // 19. APPEND THE REPOSITIONING MESSAGE
            // ==========================================
            //
            // If the move succeeds, append the movement
            // result to the existing attack message.
            //
            // This way the LAST ACTION display reports
            // both the damage and the repositioning event.

            if (moved)
            {
                if (displacedCharacter != nullptr)
                {
                    // The destination was occupied, so
                    // Formation swapped the two characters.

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
                    // The destination was empty, so the
                    // target simply moved into that slot.

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


    // ==================================================
    // 20. ENEMY TURN COMPLETE
    // ==================================================
    //
    // The attack, stress increase, and possible
    // repositioning are complete.
    //
    // Battle::start() will continue processing the round.
    // The next screen redraw will display lastActionMessage
    // and the updated formation positions.
}


// --------------------------------------------------
// Start battle
// --------------------------------------------------


void Battle::start()
{
    // ==================================================
    // 1. INITIALIZE THE BATTLE SCREEN
    // ==================================================
    //
    // Clear previous room or menu text, then display
    // the battle introduction and both formations.
    //
    // The battle's enemy party has already been generated
    // by the Battle constructor.

    clearScreen();

    std::cout << "====================================\n";
    std::cout << "          BATTLE START!\n";
    std::cout << "====================================\n";

    printBattleState();


    // ==================================================
    // 2. MAIN BATTLE LOOP
    // ==================================================
    //
    // Continue fighting while both parties have at least
    // one living character.
    //
    // playerPartyAlive() and enemyPartyAlive() check
    // the current state of each party.

    while (playerPartyAlive() && enemyPartyAlive())
    {
        // ==============================================
        // 3. RECORD THE PLAYER TURN ORDER
        // ==============================================
        //
        // Save the living player characters in their
        // current formation order at the beginning of
        // the round.
        //
        // IMPORTANT:
        // We store Character pointers, not positions.
        //
        // If a character moves during combat, their
        // pointer still refers to that same character.
        //
        // Example:
        // Initial order: Crusader, Highwayman, Plague Doctor, Vestal
        //
        // If Crusader swaps Highwayman and Plague Doctor,
        // the scheduled turn order remains unchanged.
        //
        // Their new positions affect attack and skill
        // ranges immediately, but do not change who acts
        // next during this round.

        Character* playerTurnOrder[4] = { nullptr };
        int playerTurnCount = 0;

        for (int position = 1; position <= 4; position++)
        {
            Character* character =
                playerParty.getMember(position);

            // Only living characters are scheduled.
            if (character != nullptr && character->isAlive())
            {
                playerTurnOrder[playerTurnCount] = character;
                playerTurnCount++;
            }
        }


        // ==============================================
        // 4. PROCESS PLAYER TURNS
        // ==============================================
        //
        // Process the saved pointers rather than looking
        // up a new character at each formation position.
        //
        // This prevents movement from changing the turn
        // order or causing a character to act twice.
        //
        // We still check whether the character is alive
        // because they might be defeated before their
        // scheduled turn.

        for (int i = 0; i < playerTurnCount; i++)
        {
            // End the player phase immediately if all
            // enemies have been defeated.
            if (!enemyPartyAlive())
            {
                break;
            }

            Character* character = playerTurnOrder[i];

            // A defeated character cannot take a turn.
            if (character != nullptr && character->isAlive())
            {
                playerTurn(*character);
            }
        }


        // ==============================================
        // 5. CHECK FOR PLAYER VICTORY
        // ==============================================
        //
        // If the player party defeated every enemy during
        // its phase, there is no reason to start the
        // enemy phase.

        if (!enemyPartyAlive())
        {
            break;
        }


        // ==============================================
        // 6. PROCESS ENEMY TURNS
        // ==============================================
        //
        // Enemies continue to act in formation order.
        //
        // Their turns are looked up by position, matching
        // your existing enemy-turn system.
        //
        // Movement currently affects the player formation,
        // so we can leave the enemy turn logic unchanged.

        for (int position = 1; position <= 4; position++)
        {
            // Stop the enemy phase if all heroes have
            // been defeated.
            if (!playerPartyAlive())
            {
                break;
            }

            Character* enemy =
                enemyParty.getMember(position);

            // Only living enemies can act.
            if (enemy != nullptr && enemy->isAlive())
            {
                enemyTurn(*enemy);
            }
        }


        // ==============================================
        // 7. REDRAW THE BATTLE STATE
        // ==============================================
        //
        // After both sides have acted, display the
        // updated formations, HP, stress, and positions.
        //
        // printBattleState() also displays the latest
        // action stored in lastActionMessage.

        clearScreen();
        printBattleState();
    }


    // ==================================================
    // 8. DISPLAY THE FINAL BATTLE RESULT
    // ==================================================
    //
    // The battle loop has ended because one party has
    // no living members remaining.
    //
    // Clear the screen and display the final battle
    // state before announcing victory or defeat.
    //
    // This allows the player to see the final HP values
    // and the action that ended the battle.

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
