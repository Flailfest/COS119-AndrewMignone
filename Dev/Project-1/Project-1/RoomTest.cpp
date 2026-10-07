#include "RoomTest.h"

#include "Crusader.h"
#include "Highwayman.h"
#include "PlagueDoctor.h"
#include "Vestal.h"
#include "Party.h"
#include "Room.h"
#include "Inventory.h"
#include "Input.h"

#include <iostream>

void clearRoomTestScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Character* choosePartyMember(Party& party)
{
    while (true)
    {
        std::cout << "\n";
        std::cout << "Choose a hero:\n";

        for (int i = 1; i <= 4; i++)
        {
            Character* character = party.getMember(i);

            if (character != nullptr &&
                character->isAlive())
            {
                std::cout
                    << i
                    << ". "
                    << character->getName()
                    << " ("
                    << character->getHP()
                    << "/"
                    << character->getMaxHP()
                    << " HP, "
                    << character->getStress()
                    << " Stress)\n";
            }
        }

        std::cout << "0. Back\n";

        int choice = Input::getInt(
            "\nChoose a hero: "
        );

        if (choice == 0)
        {
            return nullptr;
        }

        if (choice >= 1 && choice <= 4)
        {
            Character* character =
                party.getMember(choice);

            if (character != nullptr &&
                character->isAlive())
            {
                return character;
            }
        }

        std::cout << "Invalid target.\n";
    }
}

void openInventory(
    Inventory& inventory,
    Party& party)
{
    while (true)
    {
        clearRoomTestScreen();

        inventory.printInventory();

        std::cout << "\n";
        std::cout << "====================================\n";
        std::cout << "          INVENTORY MENU\n";
        std::cout << "====================================\n\n";

        std::cout << "1. Use Food\n";
        std::cout << "2. Use Torch\n";
        std::cout << "3. Use Medicine\n";
        std::cout << "0. Back\n";

        int choice = Input::getInt(
            "\nChoose an option: "
        );

        if (choice == 0)
        {
            return;
        }

        if (choice == 1)
        {
            if (inventory.getFood() <= 0)
            {
                std::cout << "\nYou have no food.\n";
                Input::waitForEnter();
                continue;
            }

            Character* target =
                choosePartyMember(party);

            if (target == nullptr)
            {
                continue;
            }

            inventory.useFood();

            target->heal(2);
            target->reduceStress(5);

            std::cout
                << target->getName()
                << " eats food.\n";

            std::cout
                << target->getName()
                << " recovers 2 HP and loses 5 stress.\n";

            Input::waitForEnter();
            continue;
        }

        if (choice == 2)
        {
            if (inventory.getTorches() <= 0)
            {
                std::cout << "\nYou have no torches.\n";
                Input::waitForEnter();
                continue;
            }

            inventory.useTorch();

            std::cout
                << "\nYou light a torch.\n";

            Input::waitForEnter();
            continue;
        }

        if (choice == 3)
        {
            if (inventory.getMedicine() <= 0)
            {
                std::cout << "\nYou have no medicine.\n";
                Input::waitForEnter();
                continue;
            }

            Character* target =
                choosePartyMember(party);

            if (target == nullptr)
            {
                continue;
            }

            inventory.useMedicine();

            target->heal(6);
            target->addStress(1);

            std::cout
                << target->getName()
                << " uses medicine.\n";

            std::cout
                << target->getName()
                << " recovers 6 HP and gains 1 stress.\n";

            Input::waitForEnter();
            continue;
        }

        std::cout << "\nInvalid choice.\n";
        Input::waitForEnter();
    }
}

void RoomTest::run()
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

    for (int i = 0; i < 5; i++)
    {
        Room room;

        room.enter(playerParty);

        while (true)
        {
            clearRoomTestScreen();

            std::cout << "====================================\n";
            std::cout << "          AFTER ROOM " << i + 1 << "\n";
            std::cout << "====================================\n\n";

            std::cout << "1. Continue to next room\n";
            std::cout << "2. Inventory\n";
            std::cout << "3. Leave\n";

            int choice = Input::getInt(
                "\nChoose an option: "
            );

            if (choice == 1)
            {
                break;
            }

            if (choice == 2)
            {
                openInventory(
                    inventory,
                    playerParty
                );

                continue;
            }

            if (choice == 3)
            {
                return;
            }

            std::cout << "\nInvalid choice.\n";
            Input::waitForEnter();
        }
    }

    clearRoomTestScreen();

    std::cout << "====================================\n";
    std::cout << "          ROOM TEST COMPLETE\n";
    std::cout << "====================================\n";

    Input::waitForEnter();
}