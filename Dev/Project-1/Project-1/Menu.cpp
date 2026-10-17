#include "Menu.h"
#include "Input.h"
#include "Campaign.h"
#include "Town.h"

#include <iostream>

void printTitleScreen()
{
    // ANSI colors supported by Windows Terminal.
    const char* RESET = "\033[0m";
    const char* RED = "\033[38;5;124m";
    const char* GOLD = "\033[38;5;220m";
    const char* GREY = "\033[38;5;245m";
    const char* DARK = "\033[38;5;238m";

    // Clear the terminal before drawing the title.
    std::cout << "\033[2J\033[1;1H";

    // Decorative top border.
    std::cout << DARK;
    std::cout << RESET;

    // ASCII cellar doorway.
    std::cout << RED;

    std::cout << R"(
                .-=============-.
               /                 \
              /   .-----------.   \
             |   /             \   |
             |  |   .------.    |  |
             |  |  | .--. | |   |  |
             |  |  | |  | | |   |  |
             |  |  | |  | | |   |  |
             |  |  | |__| | |   |  |
             |  |  |______| |  /   |
             |   \____________/    |
             |       | |           |
             |       |_|           |
             '====================='
)";

    std::cout << RESET;

    // Main title.
    std::cout << "\n";
    std::cout << GOLD;
    std::cout << "       V I S U A L L Y   O B S C U R E D\n";
    std::cout << RED;
    std::cout << "                    C E L L A R\n";
    std::cout << RESET;

    // Subtitle.
    std::cout << "\n";
    std::cout << GREY;
    std::cout
        << "          The darkness remembers every step.\n";
    std::cout << RESET;

    // Decorative separator.
    std::cout << "\n";
    std::cout << DARK;
    std::cout
        << "============================================================\n";
    std::cout << RESET;
}



void runMainMenu()
{
    while (true)
    {
        printTitleScreen();

        std::cout << "\n";
        std::cout << "                 MAIN MENU\n\n";
        std::cout << "          [1]  Start Campaign\n";
        std::cout << "          [2]  Exit\n";

        const int choice = Input::getIntInRange(
            "\nChoose an option (1-2): ",
            1,
            2
        );

        if (choice == 1)
        {
            Campaign campaign;
            Town town;
            town.run(campaign);
            Input::waitForEnter();
            continue;
        }

        if (choice == 2)
        {
            std::cout << "Goodbye!\n";
            return;
        }
    }
}
