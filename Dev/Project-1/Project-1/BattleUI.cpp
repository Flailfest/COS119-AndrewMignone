#include "Battle.h"
#include "BattleUI.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
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
    const std::string GOLD = "\033[38;5;180m";
    const std::string RESET = "\033[0m";

    const int panelWidth = 100;
    const int playerColumnWidth = 60;
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
                std::size_t end = start + maximumWidth;

                if (end > text.length())
                {
                    end = text.length();
                }
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
                std::cout
                    << "  "
                    << text.substr(start, end - start)
                    << "\n";
                start = end;

                while (start < text.length() &&
                    text[start] == ' ')
                {
                    start++;
                }
            }
        };
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
    for (int i = 1; i <= 4; i++)
    {
        Character* player =
            playerParty.getMember(i);

        Character* enemy =
            enemyParty.getMember(i);
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
    std::cout
        << std::string(panelWidth, '-')
        << "\n\n";
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

