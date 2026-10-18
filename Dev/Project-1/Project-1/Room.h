#pragma once

#include "Party.h"

class Inventory;

enum class RoomType
{
    Empty,
    Trap,
    Chest,
    Fountain,
    Entrance,
    Exit
};

enum class LightLevel
{
    Bright,
    Dim,
    Off
};

class Room
{
private:
    RoomType type;

    RoomType generateRandomRoom();

    bool battleOccurs() const;

    void enterInternal(Party& party, LightLevel lightLevel, Inventory* inventory, int* gold);

    void triggerEvent(Party& party, Inventory* inventory, int* gold);
    void triggerEmpty();
    void triggerTrap(Party& party);
    void triggerChest(Party& party, Inventory* inventory, int* gold);
    void triggerFountain(Party& party);

public:
    Room();
    explicit Room(RoomType roomType);

    // Legacy overload retained for any existing callers outside Floor.
    void enter(Party& party, LightLevel lightLevel);

    // Expedition overload lets chest events award gold to the player.
    void enter(Party& party, LightLevel lightLevel, Inventory& inventory, int& gold);

    RoomType getType() const;
};