#include "Room.h"
#include "Battle.h"
#include "Input.h"

#include <iostream>
#include <random>

void clearRoomScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

Room::Room()
{
    type = generateRandomRoom();
}

RoomType Room::generateRandomRoom()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> distribution(1, 4);

    int roomChoice = distribution(generator);

    switch (roomChoice)
    {
    case 1:
        return RoomType::Empty;

    case 2:
        return RoomType::Trap;

    case 3:
        return RoomType::Chest;

    case 4:
        return RoomType::Fountain;
    }

    return RoomType::Empty;
}

bool Room::battleOccurs() const
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    // 30% chance of battle
    std::uniform_int_distribution<int> distribution(1, 100);

    return distribution(generator) <= 30;
}

void Room::enter(Party& party)
{
    clearRoomScreen();

    std::cout << "====================================\n";
    std::cout << "           ENTERING ROOM\n";
    std::cout << "====================================\n\n";

    switch (type)
    {
    case RoomType::Empty:
        std::cout << "Room Type: Empty\n\n";
        break;

    case RoomType::Trap:
        std::cout << "Room Type: Trap\n\n";
        break;

    case RoomType::Chest:
        std::cout << "Room Type: Chest\n\n";
        break;

    case RoomType::Fountain:
        std::cout << "Room Type: Fountain\n\n";
        break;
    }

    std::cout << "The party moves through the room.\n";
    std::cout << "Every hero gains 5 stress.\n\n";

    for (int i = 1; i <= 4; i++)
    {
        Character* character = party.getMember(i);

        if (character != nullptr && character->isAlive())
        {
            character->addStress(5);
        }
    }

    triggerEvent(party);

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "          PARTY STRESS\n";
    std::cout << "====================================\n";

    for (int i = 1; i <= 4; i++)
    {
        Character* character = party.getMember(i);

        if (character != nullptr)
        {
            std::cout
                << character->getName()
                << ": "
                << character->getStress()
                << "/200 Stress\n";
        }
    }

    std::cout << "\n";

    if (battleOccurs())
    {
        std::cout << "The room is disturbed...\n";
        std::cout << "An enemy party appears!\n";

        Input::waitForEnter();

        Battle battle(party);
        battle.start();
    }
    else
    {
        std::cout << "The room remains quiet.\n";

        Input::waitForEnter();
    }
}

void Room::triggerEvent(Party& party)
{
    switch (type)
    {
    case RoomType::Empty:
        triggerEmpty();
        break;

    case RoomType::Trap:
        triggerTrap(party);
        break;

    case RoomType::Chest:
        triggerChest(party);
        break;

    case RoomType::Fountain:
        triggerFountain(party);
        break;
    }
}

void Room::triggerEmpty()
{
    std::cout << "Nothing of interest is found.\n";
}

void Room::triggerTrap(Party& party)
{
    std::cout << "A trap has been triggered!\n\n";

    // Additional 10 stress to every living hero.
    for (int i = 1; i <= 4; i++)
    {
        Character* character = party.getMember(i);

        if (character != nullptr && character->isAlive())
        {
            character->addStress(10);
        }
    }

    std::cout << "The entire party gains 10 additional stress.\n\n";

    // Build a list of living heroes.
    Character* targets[4];
    int targetCount = 0;

    for (int i = 1; i <= 4; i++)
    {
        Character* character = party.getMember(i);

        if (character != nullptr && character->isAlive())
        {
            targets[targetCount] = character;
            targetCount++;
        }
    }

    if (targetCount == 0)
    {
        return;
    }

    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> targetDistribution(
        0,
        targetCount - 1
    );

    Character* target =
        targets[targetDistribution(generator)];

    // Random damage from 2-5.
    std::uniform_int_distribution<int> damageDistribution(
        2,
        5
    );

    int damage = damageDistribution(generator);

    target->takeDamage(damage);
    target->addStress(5);

    std::cout
        << target->getName()
        << " takes "
        << damage
        << " damage.\n";

    std::cout
        << target->getName()
        << " gains 5 additional stress.\n";
}

void Room::triggerChest(Party& party)
{
    std::cout << "A chest has been discovered!\n";
    std::cout << "The chest has not been opened yet.\n";
}

void Room::triggerFountain(Party& party)
{
    std::cout << "A fountain has been discovered!\n\n";

    // Reduce stress for every living party member.
    for (int i = 1; i <= 4; i++)
    {
        Character* character = party.getMember(i);

        if (character != nullptr && character->isAlive())
        {
            character->reduceStress(10);
        }
    }

    std::cout << "The party's stress is reduced by 10.\n\n";

    // Display available party members.
    std::cout << "Choose a hero to heal:\n";

    for (int i = 1; i <= 4; i++)
    {
        Character* character = party.getMember(i);

        if (character != nullptr && character->isAlive())
        {
            std::cout
                << i << ". "
                << character->getName()
                << " ("
                << character->getHP()
                << "/"
                << character->getMaxHP()
                << " HP)\n";
        }
    }

    int choice = Input::getInt("Choose a hero: ");

    Character* target = party.getMember(choice);

    // Make sure the selected position contains a living hero.
    while (target == nullptr || !target->isAlive())
    {
        std::cout << "Invalid target.\n";

        choice = Input::getInt("Choose a hero: ");

        target = party.getMember(choice);
    }

    target->heal(3);

    std::cout
        << target->getName()
        << " is healed for 3 HP.\n";
}

RoomType Room::getType() const
{
    return type;
}