#pragma once

#include "Room.h" // LightLevel

class Party;
class Inventory;

// Standalone floor test plus a campaign-friendly overload that reuses state.
class FloorTest
{
public:
    void run();
    void run(Party& party, Inventory& inventory, int& gold, LightLevel& lightLevel);
};
