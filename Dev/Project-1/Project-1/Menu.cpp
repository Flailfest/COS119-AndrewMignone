#include "Menu.h"
#include "CombatTest.h"

#include <iostream>

void runMainMenu()
{
    while (true)
    {
        std::cout << "\033[2J\033[1;1H";

        std::cout << "====================================\n";
        std::cout << "      Visually Obscured Cellar\n";
        std::cout << "====================================\n\n";

        std::cout << "1. Battle Test\n";
        std::cout << "2. Exit\n";

        std::cout << "\nChoose an option: ";

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
            CombatTest();

            std::cout << "\nPress Enter to return to the main menu...";

            std::cin.ignore(10000, '\n');
            std::cin.get();

            continue;
        }

        if (choice == 2)
        {
            std::cout << "Goodbye!\n";
            return;
        }

        std::cout << "Invalid choice.\n";
    }
}
