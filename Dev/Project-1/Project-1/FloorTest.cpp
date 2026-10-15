#include "FloorTest.h"

#include "Crusader.h"
#include "Highwayman.h"
#include "PlagueDoctor.h"
#include "Vestal.h"
#include "Party.h"
#include "Room.h"
#include "Inventory.h"
#include "Input.h"
#include "Floor.h"

#include <iostream>
#include <cstdlib>
#include <string>

namespace
{
    // ANSI colors work in Windows Terminal and most modern console terminals.
    const char* const RESET = "\033[0m";
    const char* const BOLD = "\033[1m";
    const char* const DIM = "\033[38;5;240m";
    const char* const WHITE = "\033[38;5;252m";
    const char* const RED = "\033[38;5;160m";
    const char* const GREEN = "\033[38;5;114m";
    const char* const CYAN = "\033[38;5;80m";
    const char* const GOLD = "\033[38;5;220m";
    const char* const ORANGE = "\033[38;5;208m";
    const int UI_WIDTH = 58;

    void clearFloorTestScreen()
    {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }

    void printRule(char character = '=', const char* color = DIM)
    {
        std::cout << color << std::string(UI_WIDTH, character) << RESET << '\n';
    }

    void printHeader(const std::string& title, const char* color = GOLD)
    {
        printRule('=');
        std::cout << color << BOLD << "  " << title << RESET << '\n';
        printRule('=');
        std::cout << '\n';
    }

    const char* getLightLevelName(LightLevel lightLevel)
    {
        switch (lightLevel)
        {
        case LightLevel::Bright:
            return "Bright";
        case LightLevel::Dim:
            return "Dim";
        case LightLevel::Off:
            return "Off";
        }

        return "Unknown";
    }

    const char* getLightLevelColor(LightLevel lightLevel)
    {
        switch (lightLevel)
        {
        case LightLevel::Bright:
            return GREEN;
        case LightLevel::Dim:
            return GOLD;
        case LightLevel::Off:
            return RED;
        }

        return WHITE;
    }

    const char* getHealthColor(const Character& character)
    {
        const int maxHP = character.getMaxHP();
        if (maxHP <= 0 || character.getHP() * 4 <= maxHP)
        {
            return RED;
        }
        if (character.getHP() * 2 <= maxHP)
        {
            return GOLD;
        }
        return GREEN;
    }

    const char* getStressColor(const Character& character)
    {
        return character.getStress() >= 100 ? RED : CYAN;
    }

    bool partyHasLivingHeroes(Party& party)
    {
        for (int position = 1; position <= 4; ++position)
        {
            Character* character = party.getMember(position);
            if (character != nullptr && character->isAlive())
            {
                return true;
            }
        }

        return false;
    }

    Character* choosePartyMember(Party& party)
    {
        while (true)
        {
            std::cout << '\n' << CYAN << BOLD << "  CHOOSE A HERO" << RESET << '\n';
            printRule('-');

            for (int position = 1; position <= 4; ++position)
            {
                Character* character = party.getMember(position);
                if (character != nullptr && character->isAlive())
                {
                    std::cout << "  " << GOLD << position << RESET << ". "
                              << WHITE << character->getName() << RESET
                              << "  HP: " << getHealthColor(*character)
                              << character->getHP() << "/" << character->getMaxHP()
                              << RESET << "  Stress: " << getStressColor(*character)
                              << character->getStress() << RESET << '\n';
                }
            }

            std::cout << "  " << DIM << "0. Back" << RESET << '\n';
            const int choice = Input::getInt("\nChoose a hero: ");

            if (choice == 0)
            {
                return nullptr;
            }

            if (choice >= 1 && choice <= 4)
            {
                Character* character = party.getMember(choice);
                if (character != nullptr && character->isAlive())
                {
                    return character;
                }
            }

            std::cout << RED << "Invalid target.\n" << RESET;
        }
    }

    void printPartyStatus(Party& party)
    {
        clearFloorTestScreen();
        printHeader("PARTY STATUS", CYAN);

        for (int position = 1; position <= 4; ++position)
        {
            Character* character = party.getMember(position);
            if (character == nullptr)
            {
                std::cout << DIM << "  [" << position << "] Empty formation slot\n" << RESET;
                printRule('-', DIM);
                continue;
            }

            std::cout << "  " << GOLD << BOLD << '[' << position << "] "
                      << character->getName() << RESET << '\n';
            std::cout << "      Health : " << getHealthColor(*character)
                      << character->getHP() << '/' << character->getMaxHP()
                      << " HP" << RESET << '\n';
            std::cout << "      Stress : " << getStressColor(*character)
                      << character->getStress() << "/200" << RESET << '\n';
            std::cout << "      Status : "
                      << (character->isAlive() ? GREEN : RED)
                      << (character->isAlive() ? "Alive" : "Fallen")
                      << RESET << '\n';
            printRule('-', DIM);
        }

        std::cout << '\n';
        Input::waitForEnter();
    }

    void openInventory(
        Inventory& inventory,
        Party& party,
        LightLevel& lightLevel,
        int& gold
    )
    {
        while (true)
        {
            clearFloorTestScreen();
            printHeader("EXPEDITION INVENTORY", GOLD);

            std::cout << CYAN << BOLD << "  SUPPLIES" << RESET << '\n';
            printRule('-', DIM);
            std::cout << "  Food       : " << GREEN << inventory.getFood() << RESET << '\n';
            std::cout << "  Torches    : " << GOLD << inventory.getTorches() << RESET << '\n';
            std::cout << "  Medicine   : " << CYAN << inventory.getMedicine() << RESET << '\n';
            std::cout << "  Gold       : " << GOLD << BOLD << gold << RESET << '\n';
            std::cout << "  Torchlight : " << getLightLevelColor(lightLevel)
                      << getLightLevelName(lightLevel) << RESET << "\n\n";

            printRule('=');
            std::cout << CYAN << BOLD << "  USE AN ITEM" << RESET << '\n';
            printRule('-', DIM);
            std::cout << "  " << GOLD << "1" << RESET << ". Use Food       "
                      << DIM << "(heal 2 HP, -5 stress)" << RESET << '\n';
            std::cout << "  " << GOLD << "2" << RESET << ". Use Torch      "
                      << DIM << "(restore one light level)" << RESET << '\n';
            std::cout << "  " << GOLD << "3" << RESET << ". Use Medicine   "
                      << DIM << "(heal 6 HP, +1 stress)" << RESET << '\n';
            std::cout << "  " << DIM << "0. Back" << RESET << '\n';
            printRule('=');

            const int choice = Input::getInt("\nChoose an option: ");
            if (choice == 0)
            {
                return;
            }

            if (choice == 1)
            {
                if (inventory.getFood() <= 0)
                {
                    std::cout << RED << "\nYou have no food.\n" << RESET;
                    Input::waitForEnter();
                    continue;
                }

                Character* target = choosePartyMember(party);
                if (target == nullptr)
                {
                    continue;
                }

                inventory.useFood();
                target->heal(2);
                target->reduceStress(5);
                std::cout << GREEN << "\n" << target->getName()
                          << " eats food, recovers 2 HP, and loses 5 stress.\n" << RESET;
                Input::waitForEnter();
                continue;
            }

            if (choice == 2)
            {
                if (inventory.getTorches() <= 0)
                {
                    std::cout << RED << "\nYou have no torches.\n" << RESET;
                    Input::waitForEnter();
                    continue;
                }

                if (lightLevel == LightLevel::Bright)
                {
                    std::cout << GREEN << "\nThe torchlight is already bright.\n" << RESET;
                    Input::waitForEnter();
                    continue;
                }

                inventory.useTorch();
                if (lightLevel == LightLevel::Off)
                {
                    lightLevel = LightLevel::Dim;
                }
                else if (lightLevel == LightLevel::Dim)
                {
                    lightLevel = LightLevel::Bright;
                }

                std::cout << "\nYou use a torch. Torchlight is now "
                          << getLightLevelColor(lightLevel) << getLightLevelName(lightLevel)
                          << RESET << ".\n";
                Input::waitForEnter();
                continue;
            }

            if (choice == 3)
            {
                if (inventory.getMedicine() <= 0)
                {
                    std::cout << RED << "\nYou have no medicine.\n" << RESET;
                    Input::waitForEnter();
                    continue;
                }

                Character* target = choosePartyMember(party);
                if (target == nullptr)
                {
                    continue;
                }

                inventory.useMedicine();
                target->heal(6);
                target->addStress(1);
                std::cout << GREEN << "\n" << target->getName()
                          << " uses medicine, recovers 6 HP, and gains 1 stress.\n" << RESET;
                Input::waitForEnter();
                continue;
            }

            std::cout << RED << "\nInvalid choice.\n" << RESET;
            Input::waitForEnter();
        }
    }

    void showFloorMap(Floor& floor, LightLevel lightLevel)
    {
        clearFloorTestScreen();
        floor.printMap();
        std::cout << "\nCurrent torchlight: " << getLightLevelColor(lightLevel)
                  << BOLD << getLightLevelName(lightLevel) << RESET << "\n\n";
        Input::waitForEnter();
    }
}

void FloorTest::run()
{
    Crusader crusader;
    Highwayman highwayman;
    PlagueDoctor plagueDoctor;
    Vestal vestal;

    Party playerParty;
    playerParty.addMember(crusader, 1);
    playerParty.addMember(highwayman, 2);
    playerParty.addMember(plagueDoctor, 3);
    playerParty.addMember(vestal, 4);

    Inventory inventory(5, 3, 2);
    int gold = 0;
    LightLevel lightLevel = LightLevel::Bright;
    Floor floor;

    while (true)
    {
        if (!partyHasLivingHeroes(playerParty))
        {
            clearFloorTestScreen();
            printHeader("EXPEDITION FAILED", RED);
            std::cout << "  Every hero has fallen in the cellar.\n\n";
            Input::waitForEnter();
            return;
        }

        if (floor.isAtExit())
        {
            clearFloorTestScreen();
            printHeader("FLOOR CLEARED", GREEN);
            std::cout << "  The party found the stairs leading deeper into the cellar.\n\n";
            std::cout << "  Gold collected     : " << GOLD << BOLD << gold << RESET << '\n';
            std::cout << "  Rooms on this floor: " << CYAN << floor.getRoomCount() << RESET << "\n\n";
            printRule('=');
            Input::waitForEnter();
            return;
        }

        clearFloorTestScreen();
        printRule('=');
        std::cout << RED << BOLD << "  VISUALLY OBSCURED CELLAR" << RESET << '\n';
        std::cout << GOLD << BOLD << "  FLOOR EXPEDITION" << RESET << '\n';
        printRule('=');
        std::cout << '\n';

        if (floor.getCurrentRoomId() == 0)
        {
            std::cout << "  Location   : " << GREEN << BOLD << "Entrance" << RESET << '\n';
        }
        else
        {
            std::cout << "  Location   : " << CYAN << BOLD << "Room "
                      << floor.getCurrentRoomId() << RESET << '\n';
        }

        std::cout << "  Torchlight : " << getLightLevelColor(lightLevel)
                  << BOLD << getLightLevelName(lightLevel) << RESET << '\n';
        std::cout << "  Gold       : " << GOLD << BOLD << gold << RESET << "\n\n";

        printRule('-');
        std::cout << CYAN << BOLD << "  EXPEDITION OPTIONS" << RESET << '\n';
        printRule('-');
        std::cout << "  " << GOLD << "1" << RESET << ". Move through a hallway\n";
        std::cout << "  " << GOLD << "2" << RESET << ". Inventory\n";
        std::cout << "  " << GOLD << "3" << RESET << ". Party Status\n";
        std::cout << "  " << GOLD << "4" << RESET << ". Floor Map\n";
        std::cout << "  " << RED << "5" << RESET << ". Leave Expedition\n";
        printRule('=');

        const int choice = Input::getInt("\nChoose an option: ");

        if (choice == 1)
        {
            clearFloorTestScreen();
            printHeader("HALLWAY", CYAN);
            floor.printRoomChoices();
            std::cout << '\n';
            printRule('-');
            std::cout << DIM << "  Moving through a hallway drains torchlight by one level.\n"
                      << "  Enter -1 to stay where you are." << RESET << "\n";
            printRule('-');

            const int destination = Input::getInt("\nChoose destination room ID: ");
            if (destination == -1)
            {
                continue;
            }

            if (!floor.moveToRoom(destination, lightLevel))
            {
                std::cout << RED << "\nThat room is not connected to your current location.\n" << RESET;
                Input::waitForEnter();
                continue;
            }

            std::cout << "\nYou travel through the hallway. Torchlight is now "
                      << getLightLevelColor(lightLevel) << BOLD
                      << getLightLevelName(lightLevel) << RESET << ".\n";

            if (!floor.hasEntered(floor.getCurrentRoomId()))
            {
                floor.enterCurrentRoom(playerParty, lightLevel, inventory, gold);
            }
            else
            {
                std::cout << CYAN << "You return to a room you have already explored.\n" << RESET;
                Input::waitForEnter();
            }

            continue;
        }

        if (choice == 2)
        {
            openInventory(inventory, playerParty, lightLevel, gold);
            continue;
        }

        if (choice == 3)
        {
            printPartyStatus(playerParty);
            continue;
        }

        if (choice == 4)
        {
            showFloorMap(floor, lightLevel);
            continue;
        }

        if (choice == 5)
        {
            return;
        }

        std::cout << RED << "\nInvalid choice.\n" << RESET;
        Input::waitForEnter();
    }
}
