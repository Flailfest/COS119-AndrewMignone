#include "RoomTest.h"

#include "Crusader.h"
#include "Highwayman.h"
#include "PlagueDoctor.h"
#include "Vestal.h"
#include "Party.h"
#include "Room.h"

#include <iostream>

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

    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "             ROOM TEST\n";
    std::cout << "====================================\n";

    for (int i = 0; i < 5; i++)
    {
        std::cout << "\n";
        std::cout << "Room " << i + 1 << "\n";

        Room room;
        room.enter(playerParty);

        std::cout << "\n";
        std::cout << "------------------------------------\n";

        std::cout << "Press Enter for the next room...";

        std::string input;
        std::getline(std::cin, input);
    }
}