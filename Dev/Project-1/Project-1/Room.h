#pragma once

#include "Party.h"

enum class RoomType
{
    Empty,
    Trap,
    Chest,
    Fountain
};

class Room
{
private:
    RoomType type;

    RoomType generateRandomRoom();

    bool battleOccurs() const;

    void triggerEvent(Party& party);
    void triggerEmpty();
    void triggerTrap(Party& party);
    void triggerChest(Party& party);
    void triggerFountain(Party& party);

public:
    Room();

    void enter(Party& party);

    RoomType getType() const;
};