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

namespace
{
    void clearFloorTestScreen()
    {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
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
            std::cout << "\nChoose a hero:\n";

            for (int position = 1; position <= 4; ++position)
            {
                Character* character = party.getMember(position);

                if (character != nullptr && character->isAlive())
                {
                    std::cout
                        << position << ". "
                        << character->getName()
                        << " (" << character->getHP()
                        << "/" << character->getMaxHP()
                        << " HP, " << character->getStress()
                        << " Stress)\n";
                }
            }

            std::cout << "0. Back\n";
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

            std::cout << "Invalid target.\n";
        }
    }

    void printPartyStatus(Party& party)
    {
        clearFloorTestScreen();
        std::cout << "========================================\n";
        std::cout << "               PARTY STATUS\n";
        std::cout << "========================================\n\n";

        for (int position = 1; position <= 4; ++position)
        {
            Character* character = party.getMember(position);
            if (character == nullptr)
            {
                std::cout << "Position " << position << ": Empty\n\n";
                continue;
            }

            std::cout << position << ". " << character->getName() << '\n';
            std::cout << "   Health: " << character->getHP()
                      << "/" << character->getMaxHP() << " HP\n";
            std::cout << "   Stress: " << character->getStress()
                      << "/200\n";
            std::cout << "   Status: "
                      << (character->isAlive() ? "Alive" : "Fallen")
                      << "\n\n";
        }

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
            std::cout << "========================================\n";
            std::cout << "                 INVENTORY\n";
            std::cout << "========================================\n\n";

            inventory.printInventory();
            std::cout << "Gold: " << gold << '\n';
            std::cout << "Torchlight: " << getLightLevelName(lightLevel) << "\n\n";

            std::cout << "1. Use Food       (heal 2 HP, -5 stress)\n";
            std::cout << "2. Use Torch      (restore one light level)\n";
            std::cout << "3. Use Medicine   (heal 6 HP, +1 stress)\n";
            std::cout << "0. Back\n";

            const int choice = Input::getInt("\nChoose an option: ");
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

                Character* target = choosePartyMember(party);
                if (target == nullptr)
                {
                    continue;
                }

                inventory.useFood();
                target->heal(2);
                target->reduceStress(5);
                std::cout << "\n" << target->getName()
                          << " eats food, recovers 2 HP, and loses 5 stress.\n";
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

                if (lightLevel == LightLevel::Bright)
                {
                    std::cout << "\nThe torchlight is already bright.\n";
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
                          << getLightLevelName(lightLevel) << ".\n";
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

                Character* target = choosePartyMember(party);
                if (target == nullptr)
                {
                    continue;
                }

                inventory.useMedicine();
                target->heal(6);
                target->addStress(1);
                std::cout << "\n" << target->getName()
                          << " uses medicine, recovers 6 HP, and gains 1 stress.\n";
                Input::waitForEnter();
                continue;
            }

            std::cout << "\nInvalid choice.\n";
            Input::waitForEnter();
        }
    }

    void showFloorMap(Floor& floor, LightLevel lightLevel)
    {
        clearFloorTestScreen();
        floor.printMap();
        std::cout << "\nCurrent torchlight: " << getLightLevelName(lightLevel) << "\n";
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
            std::cout << "========================================\n";
            std::cout << "           EXPEDITION FAILED\n";
            std::cout << "========================================\n\n";
            std::cout << "Every hero has fallen in the cellar.\n";
            Input::waitForEnter();
            return;
        }

        if (floor.isAtExit())
        {
            clearFloorTestScreen();
            std::cout << "========================================\n";
            std::cout << "             FLOOR CLEARED\n";
            std::cout << "========================================\n\n";
            std::cout << "The party found the stairs leading deeper into the cellar.\n";
            std::cout << "Gold collected: " << gold << '\n';
            std::cout << "Rooms on this floor: " << floor.getRoomCount() << '\n';
            Input::waitForEnter();
            return;
        }

        clearFloorTestScreen();
        std::cout << "========================================\n";
        std::cout << "         VISUALLY OBSCURED CELLAR\n";
        std::cout << "             FLOOR EXPEDITION\n";
        std::cout << "========================================\n\n";

        if (floor.getCurrentRoomId() == 0)
        {
            std::cout << "Location: Entrance\n";
        }
        else
        {
            std::cout << "Location: Room " << floor.getCurrentRoomId() << '\n';
        }

        std::cout << "Torchlight: " << getLightLevelName(lightLevel) << '\n';
        std::cout << "Gold: " << gold << "\n\n";

        std::cout << "1. Move through a hallway\n";
        std::cout << "2. Inventory\n";
        std::cout << "3. Party Status\n";
        std::cout << "4. Floor Map\n";
        std::cout << "5. Leave Expedition\n";

        const int choice = Input::getInt("\nChoose an option: ");

        if (choice == 1)
        {
            clearFloorTestScreen();
            std::cout << "========================================\n";
            std::cout << "             HALLWAY\n";
            std::cout << "========================================\n\n";
            floor.printRoomChoices();
            std::cout << "\nMoving through a hallway drains torchlight by one level.\n";
            std::cout << "Enter -1 to stay where you are.\n";

            const int destination = Input::getInt("\nChoose destination room ID: ");
            if (destination == -1)
            {
                continue;
            }

            if (!floor.moveToRoom(destination, lightLevel))
            {
                std::cout << "\nThat room is not connected to your current location.\n";
                Input::waitForEnter();
                continue;
            }

            std::cout << "\nYou travel through the hallway. Torchlight is now "
                      << getLightLevelName(lightLevel) << ".\n";

            if (!floor.hasEntered(floor.getCurrentRoomId()))
            {
                floor.enterCurrentRoom(playerParty, lightLevel, inventory, gold);
            }
            else
            {
                std::cout << "You return to a room you have already explored.\n";
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

        std::cout << "\nInvalid choice.\n";
        Input::waitForEnter();
    }
}
