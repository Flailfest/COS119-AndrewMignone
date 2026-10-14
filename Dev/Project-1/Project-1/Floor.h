#pragma once

#include "Room.h"

#include <vector>

class Inventory;

// One room on the floor graph. Room IDs are the indices used by Floor.
struct FloorRoom
{
    int id;
    int x;
    int y;
    Room room;
    std::vector<int> connections;
    bool visited;
    bool entered;

    FloorRoom(int roomId, int gridX, int gridY, const Room& roomValue);
};

class Floor
{
private:
    std::vector<FloorRoom> rooms;
    int entranceRoomId;
    int exitRoomId;
    int currentRoomId;

    void generateFloor();
    void connectRooms(int roomA, int roomB);

public:
    Floor();

    int getCurrentRoomId() const;
    int getRoomCount() const;
    bool isAtExit() const;
    bool isVisited(int roomId) const;
    bool hasEntered(int roomId) const;
    bool isExitRoom(int roomId) const;

    const std::vector<int>& getConnections(int roomId) const;

    // Moves only to a directly connected room and drains one torchlight level.
    bool moveToRoom(int destinationRoomId, LightLevel& lightLevel);

    // Enters a room once; revisiting a room will not repeat its event or battle.
    void enterCurrentRoom(
        Party& party,
        LightLevel lightLevel,
        Inventory& inventory,
        int& gold
    );

    void printMap() const;
    void printRoomChoices() const;
};
