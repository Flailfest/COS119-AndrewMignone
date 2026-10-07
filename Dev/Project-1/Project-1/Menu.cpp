#include "Menu.h"
#include "CombatTest.h"
#include "Input.h"
#include "RoomTest.h"

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
        std::cout << "2. Room Test\n";
        std::cout << "3. Exit\n";
        

        int choice = Input::getIntInRange(
            "\nChoose an option: ",
            1,
            3
        );

        if (choice == 1)
        {
            CombatTest();

            Input::waitForEnter();

            continue;
        }
        if (choice == 2)
        {
            RoomTest roomTest;
            roomTest.run();

            Input::waitForEnter();

            continue;
        }

        if (choice == 3)
        {
            std::cout << "Goodbye!\n";
            return;
        }
    }
}