#include "Vestal.h"
#include "Crusader.h"
#include "Highwayman.h"
#include "PlagueDoctor.h"
#include "Party.h"
#include "Battle.h"
#include "Enemy.h"
#include "Menu.h"
#include "ConsoleCentering.h"


#include <iostream>

int main()
{
    // Center std::cout output from all menus, battles, rooms, and campaign screens.
    ConsoleCentering centerConsole;

    runMainMenu();

    return 0;
}