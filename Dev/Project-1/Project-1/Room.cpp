#include "Room.h"
#include "Battle.h"

#include <iostream>
#include <random>

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
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "           ENTERING ROOM\n";
    std::cout << "====================================\n";

    triggerEvent(party);

    std::cout << "\n";

    if (battleOccurs())
    {
        std::cout << "The room is disturbed...\n";
        std::cout << "An enemy party appears!\n";

        Battle battle(party);
        battle.start();
    }
    else
    {
        std::cout << "The room is quiet.\n";
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
    std::cout << "The room is empty.\n";
}

void Room::triggerTrap(Party& party)
{
    std::cout << "You found a trap!\n";

    // Temporary placeholder.
    // We will add actual trap behavior later.
}

void Room::triggerChest(Party& party)
{
    std::cout << "You found a chest!\n";

    // Temporary placeholder.
    // We will add rewards later.
}

void Room::triggerFountain(Party& party)
{
    std::cout << "You found a fountain!\n";

    // Temporary placeholder.
    // We will add healing/stress reduction later.
}

RoomType Room::getType() const
{
    return type;
}