#include "Battle.h"
#include "BattleRules.h"
#include "BattleUI.h"
#include "Input.h"

#include <iostream>
#include <string>

bool Battle::movePartyMember()
{
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
        int fromPosition = Input::getIntInRange(
            "\nSelect a position to move: ",
            0,
            4
        );

        if (fromPosition == 0)
        {
            return false;
        }
        Character* movingCharacter =
            playerParty.getMember(fromPosition);
        if (movingCharacter == nullptr)
        {
            std::cout << "There is no character in that position.\n";

            Input::waitForEnter();
            continue;
        }
        if (!movingCharacter->isAlive())
        {
            std::cout << "A defeated character cannot be moved.\n";

            Input::waitForEnter();
            continue;
        }
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
        if (fromPosition == toPosition)
        {
            std::cout
                << "That character is already in that position.\n";

            Input::waitForEnter();
            continue;
        }
        Character* displacedCharacter =
            playerParty.getMember(toPosition);
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
        return true;
    }
}


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

            if (!BattleRules::isPositionInRange(
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

        if (!BattleRules::isPositionInRange(
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


bool Battle::attackMenu(Character& character)
{
    BattleRules::PositionRules rules;

    if (!BattleRules::getAttackRules(character, rules))
    {
        std::cout
            << "No basic attack rules exist for this character.\n";

        Input::waitForEnter();

        return false;
    }
    int userPosition = playerParty.getPosition(character);

    if (!BattleRules::isPositionInRange(
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
    Character* target = chooseEnemyTarget(
        character,
        rules.targetMin,
        rules.targetMax
    );

    if (target == nullptr)
    {
        return false;
    }
    int hpBefore = target->getHP();
    character.Character::attack(*target);
    int damageDealt = hpBefore - target->getHP();
    lastActionMessage =
        character.getName() +
        " attacks " +
        target->getName() +
        " for " +
        std::to_string(damageDealt) +
        " damage.";
    if (!target->isAlive())
    {
        lastActionMessage +=
            " " +
            target->getName() +
            " has been defeated!";
    }
    return true;
}


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
        BattleRules::PositionRules rules;

        if (!BattleRules::getSkillRules(character, choice, rules))
        {
            std::cout << "Invalid skill.\n";

            Input::waitForEnter();

            continue;
        }
        int userPosition =
            playerParty.getPosition(character);

        if (!BattleRules::isPositionInRange(
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
        int hpBefore = target->getHP();
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
        character.useSkill(choice, *target);
        int hpAfter = target->getHP();
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
        return;
    }
}

