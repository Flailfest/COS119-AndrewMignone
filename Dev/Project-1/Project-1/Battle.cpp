#include "Battle.h"
#include "BattleUI.h"

#include <iostream>

void Battle::start()
{
    clearScreen();

    std::cout << "====================================\n";
    std::cout << "          BATTLE START!\n";
    std::cout << "====================================\n";

    printBattleState();
    while (playerPartyAlive() && enemyPartyAlive())
    {
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
        if (!playerPartyAlive() || !enemyPartyAlive())
        {
            break;
        }
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
        if (!playerPartyAlive() || !enemyPartyAlive())
        {
            break;
        }
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
        if (!playerPartyAlive() || !enemyPartyAlive())
        {
            break;
        }
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
        clearScreen();
        printBattleState();
    }
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


Party& Battle::getPlayerParty()
{
    return playerParty;
}


Party& Battle::getEnemyParty()
{
    return enemyParty;
}

