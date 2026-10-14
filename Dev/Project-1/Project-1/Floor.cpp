#include "Floor.h"

#include "Inventory.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

namespace
{
    const int FLOOR_WIDTH = 7;
    const int MIDDLE_ROW = 1;

    int randomInt(int minimum, int maximum)
    {
        static std::random_device rd;
        static std::mt19937 generator(rd());
        std::uniform_int_distribution<int> distribution(minimum, maximum);
        return distribution(generator);
    }

}

FloorRoom::FloorRoom(
    int roomId,
    int gridX,
    int gridY,
    const Room& roomValue
)
    : id(roomId),
      x(gridX),
      y(gridY),
      room(roomValue),
      visited(false),
      entered(false)
{
}

Floor::Floor()
    : entranceRoomId(0),
      exitRoomId(0),
      currentRoomId(0)
{
    generateFloor();
}

void Floor::generateFloor()
{
    rooms.clear();

    // The seven-room central path guarantees that every floor has a route
    // from its entrance to its exit.
    rooms.emplace_back(0, 0, MIDDLE_ROW, Room(RoomType::Entrance));
    entranceRoomId = 0;

    for (int x = 1; x < FLOOR_WIDTH - 1; ++x)
    {
        const int id = static_cast<int>(rooms.size());
        rooms.emplace_back(id, x, MIDDLE_ROW, Room());
    }

    exitRoomId = static_cast<int>(rooms.size());
    rooms.emplace_back(exitRoomId, FLOOR_WIDTH - 1, MIDDLE_ROW, Room(RoomType::Exit));

    // Add optional rooms above and below the main path. Each branch room
    // connects to the main path, and adjacent branch rooms can connect too.
    bool branchAdded = false;

    for (int y = 0; y <= 2; y += 2)
    {
        for (int x = 1; x < FLOOR_WIDTH - 1; ++x)
        {
            if (randomInt(1, 100) <= 35)
            {
                const int id = static_cast<int>(rooms.size());
                rooms.emplace_back(id, x, y, Room());
                branchAdded = true;
            }
        }
    }

    // Avoid an uninteresting straight corridor on the occasional floor with
    // no randomly selected branches.
    if (!branchAdded)
    {
        const int id = static_cast<int>(rooms.size());
        rooms.emplace_back(id, FLOOR_WIDTH / 2, 0, Room());
    }

    // Connect any rooms whose grid coordinates touch orthogonally.
    for (int i = 0; i < static_cast<int>(rooms.size()); ++i)
    {
        for (int j = i + 1; j < static_cast<int>(rooms.size()); ++j)
        {
            const int distance =
                std::abs(rooms[i].x - rooms[j].x) +
                std::abs(rooms[i].y - rooms[j].y);

            if (distance == 1)
            {
                connectRooms(i, j);
            }
        }
    }

    currentRoomId = entranceRoomId;
    rooms[entranceRoomId].visited = true;
    rooms[entranceRoomId].entered = true;
}

void Floor::connectRooms(int roomA, int roomB)
{
    if (roomA < 0 || roomB < 0 ||
        roomA >= static_cast<int>(rooms.size()) ||
        roomB >= static_cast<int>(rooms.size()))
    {
        return;
    }

    std::vector<int>& connectionsA = rooms[roomA].connections;
    std::vector<int>& connectionsB = rooms[roomB].connections;

    if (std::find(connectionsA.begin(), connectionsA.end(), roomB) == connectionsA.end())
    {
        connectionsA.push_back(roomB);
    }

    if (std::find(connectionsB.begin(), connectionsB.end(), roomA) == connectionsB.end())
    {
        connectionsB.push_back(roomA);
    }
}

int Floor::getCurrentRoomId() const
{
    return currentRoomId;
}

int Floor::getRoomCount() const
{
    return static_cast<int>(rooms.size());
}

bool Floor::isAtExit() const
{
    return currentRoomId == exitRoomId;
}

bool Floor::isVisited(int roomId) const
{
    return roomId >= 0 &&
        roomId < static_cast<int>(rooms.size()) &&
        rooms[roomId].visited;
}

bool Floor::hasEntered(int roomId) const
{
    return roomId >= 0 &&
        roomId < static_cast<int>(rooms.size()) &&
        rooms[roomId].entered;
}

bool Floor::isExitRoom(int roomId) const
{
    return roomId == exitRoomId;
}

const std::vector<int>& Floor::getConnections(int roomId) const
{
    static const std::vector<int> noConnections;

    if (roomId < 0 || roomId >= static_cast<int>(rooms.size()))
    {
        return noConnections;
    }

    return rooms[roomId].connections;
}

bool Floor::moveToRoom(int destinationRoomId, LightLevel& lightLevel)
{
    if (destinationRoomId < 0 || destinationRoomId >= static_cast<int>(rooms.size()))
    {
        return false;
    }

    const std::vector<int>& connections = rooms[currentRoomId].connections;
    if (std::find(connections.begin(), connections.end(), destinationRoomId) == connections.end())
    {
        return false;
    }

    // Traversing a hallway reduces light by one step. Off stays Off until a
    // torch is used from the inventory screen.
    if (lightLevel == LightLevel::Bright)
    {
        lightLevel = LightLevel::Dim;
    }
    else if (lightLevel == LightLevel::Dim)
    {
        lightLevel = LightLevel::Off;
    }

    currentRoomId = destinationRoomId;
    rooms[currentRoomId].visited = true;
    return true;
}

void Floor::enterCurrentRoom(
    Party& party,
    LightLevel lightLevel,
    Inventory& inventory,
    int& gold
)
{
    if (currentRoomId < 0 || currentRoomId >= static_cast<int>(rooms.size()))
    {
        return;
    }

    FloorRoom& currentRoom = rooms[currentRoomId];
    if (currentRoom.entered)
    {
        return;
    }

    // Mark it first so room effects cannot accidentally be repeated.
    currentRoom.entered = true;
    currentRoom.room.enter(party, lightLevel, inventory, gold);
}

void Floor::printMap() const
{
    const int rowCount = 5;
    const int canvasWidth = FLOOR_WIDTH * 7;
    std::vector<std::string> canvas(rowCount, std::string(canvasWidth, ' '));
    bool hasNodeOnRow[3] = { false, false, false };

    for (std::size_t i = 0; i < rooms.size(); ++i)
    {
        const FloorRoom& room = rooms[i];
        hasNodeOnRow[room.y] = true;

        std::string label;
        if (room.id == currentRoomId && room.id == entranceRoomId)
        {
            label = "@IN ";
        }
        else if (room.id == entranceRoomId)
        {
            label = "<IN>";
        }
        else if (room.id == exitRoomId)
        {
            label = "<EX>";
        }
        else if (room.id == currentRoomId)
        {
            label = "<@ >";
        }
        else
        {
            std::ostringstream labelStream;
            if (room.visited)
            {
                labelStream << '<' << std::setw(2) << std::setfill('0') << room.id << '>';
            }
            else
            {
                labelStream << '[' << std::setw(2) << std::setfill('0') << room.id << ']';
            }
            label = labelStream.str();
        }

        const int start = room.x * 7;
        for (std::size_t character = 0; character < label.size(); ++character)
        {
            canvas[room.y * 2][start + static_cast<int>(character)] = label[character];
        }
    }

    // Draw connectors from the actual graph, so the map reflects valid moves.
    for (std::size_t i = 0; i < rooms.size(); ++i)
    {
        const FloorRoom& room = rooms[i];
        for (std::size_t c = 0; c < room.connections.size(); ++c)
        {
            const FloorRoom& neighbor = rooms[room.connections[c]];

            if (neighbor.x == room.x + 1 && neighbor.y == room.y)
            {
                const int start = room.x * 7 + 4;
                const int row = room.y * 2;
                canvas[row][start] = '-';
                canvas[row][start + 1] = '-';
                canvas[row][start + 2] = '-';
            }
            else if (neighbor.y == room.y + 1 && neighbor.x == room.x)
            {
                const int row = room.y * 2 + 1;
                const int column = room.x * 7 + 2;
                canvas[row][column] = '|';
            }
        }
    }

    std::cout << "========================================\n";
    std::cout << "                 FLOOR MAP\n";
    std::cout << "========================================\n\n";

    for (int y = 0; y < 3; ++y)
    {
        if (!hasNodeOnRow[y])
        {
            continue;
        }

        std::cout << canvas[y * 2] << '\n';

        if (y < 2 && hasNodeOnRow[y + 1])
        {
            std::cout << canvas[y * 2 + 1] << '\n';
        }
    }

    std::cout << "\nLegend: @IN Current entrance  <@ > You  <IN> Entrance  <EX> Exit\n";
    std::cout << "       [ID] Unexplored room  <ID> Visited room\n";
    std::cout << "Room IDs match the movement menu. Hallways show available connections.\n";
}

void Floor::printRoomChoices() const
{
    const std::vector<int>& connections = rooms[currentRoomId].connections;

    std::cout << "Connected rooms:\n";
    for (std::size_t i = 0; i < connections.size(); ++i)
    {
        const int destination = connections[i];
        std::cout << "  " << destination << ". ";

        if (destination == entranceRoomId)
        {
            std::cout << "Entrance";
        }
        else if (destination == exitRoomId)
        {
            std::cout << "Floor Exit";
        }
        else if (rooms[destination].visited)
        {
            std::cout << "Visited room";
        }
        else
        {
            std::cout << "Unexplored room";
        }

        std::cout << '\n';
    }
}
