#include "Room.h"
#include "Battle.h"
#include "Input.h"
#include "Inventory.h"

#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

namespace
{
    // ANSI colors match the Floor Test screens. Windows Terminal supports these
    // escape sequences, as do most modern terminal applications.
    const char* const RESET  = "\033[0m";
    const char* const BOLD   = "\033[1m";
    const char* const DIM    = "\033[38;5;240m";
    const char* const WHITE  = "\033[38;5;252m";
    const char* const RED    = "\033[38;5;160m";
    const char* const GREEN  = "\033[38;5;114m";
    const char* const CYAN   = "\033[38;5;80m";
    const char* const GOLD   = "\033[38;5;220m";
    const char* const ORANGE = "\033[38;5;208m";
    const char* const BLUE   = "\033[38;5;75m";
    const int UI_WIDTH = 58;

    void clearRoomScreen()
    {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }

    void printRule(char character = '=', const char* color = DIM)
    {
        std::cout << color << std::string(UI_WIDTH, character) << RESET << '\n';
    }

    void printHeader(const std::string& title, const char* color = GOLD)
    {
        printRule('=', color);
        std::cout << color << BOLD << "  " << title << RESET << '\n';
        printRule('=', color);
    }

    const char* roomTypeName(RoomType type)
    {
        switch (type)
        {
        case RoomType::Empty:    return "Empty Room";
        case RoomType::Trap:     return "Trap";
        case RoomType::Chest:    return "Treasure Chest";
        case RoomType::Fountain: return "Fountain";
        case RoomType::Entrance: return "Entrance";
        case RoomType::Exit:     return "Floor Exit";
        }
        return "Unknown Room";
    }

    const char* roomTypeColor(RoomType type)
    {
        switch (type)
        {
        case RoomType::Empty:    return CYAN;
        case RoomType::Trap:     return RED;
        case RoomType::Chest:    return GOLD;
        case RoomType::Fountain: return GREEN;
        case RoomType::Entrance: return BLUE;
        case RoomType::Exit:     return ORANGE;
        }
        return WHITE;
    }

    const char* lightLevelName(LightLevel lightLevel)
    {
        switch (lightLevel)
        {
        case LightLevel::Bright: return "Bright";
        case LightLevel::Dim:    return "Dim";
        case LightLevel::Off:    return "Off";
        }
        return "Unknown";
    }

    const char* lightLevelColor(LightLevel lightLevel)
    {
        switch (lightLevel)
        {
        case LightLevel::Bright: return GREEN;
        case LightLevel::Dim:    return GOLD;
        case LightLevel::Off:    return RED;
        }
        return WHITE;
    }

    const char* healthColor(const Character& character)
    {
        const int maxHP = character.getMaxHP();
        if (maxHP <= 0 || character.getHP() * 4 <= maxHP)
        {
            return RED;
        }
        if (character.getHP() * 2 <= maxHP)
        {
            return GOLD;
        }
        return GREEN;
    }

    const char* stressColor(const Character& character)
    {
        return character.getStress() >= 100 ? RED : CYAN;
    }
}

Room::Room()
{
    type = generateRandomRoom();
}

Room::Room(RoomType roomType)
    : type(roomType)
{
}

RoomType Room::generateRandomRoom()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> distribution(1, 4);

    switch (distribution(generator))
    {
    case 1: return RoomType::Empty;
    case 2: return RoomType::Trap;
    case 3: return RoomType::Chest;
    case 4: return RoomType::Fountain;
    }

    return RoomType::Empty;
}

bool Room::battleOccurs() const
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, 100);
    return distribution(generator) <= 30;
}

void Room::enter(Party& party, LightLevel lightLevel)
{
    enterInternal(party, lightLevel, nullptr, nullptr);
}

void Room::enter(Party& party, LightLevel lightLevel, Inventory& inventory, int& gold)
{
    enterInternal(party, lightLevel, &inventory, &gold);
}

void Room::enterInternal(
    Party& party,
    LightLevel lightLevel,
    Inventory* inventory,
    int* gold
)
{
    clearRoomScreen();
    printHeader("ROOM ENCOUNTER", GOLD);

    std::cout << "  Room type  : " << roomTypeColor(type) << BOLD
              << roomTypeName(type) << RESET << '\n';
    std::cout << "  Torchlight : " << lightLevelColor(lightLevel) << BOLD
              << lightLevelName(lightLevel) << RESET << '\n';

    if (type == RoomType::Exit)
    {
        printRule('-');
        std::cout << GREEN << BOLD
                  << "  The stairs lead deeper into the cellar.\n"
                  << RESET;
        printRule('=');
        Input::waitForEnter();
        return;
    }

    int stressAmount = 0;
    switch (lightLevel)
    {
    case LightLevel::Bright: stressAmount = 2; break;
    case LightLevel::Dim:    stressAmount = 5; break;
    case LightLevel::Off:    stressAmount = 10; break;
    }

    std::cout << "  Light effect: each living hero gains "
              << lightLevelColor(lightLevel) << BOLD << '+' << stressAmount
              << " stress" << RESET << "\n";

    for (int i = 1; i <= 4; ++i)
    {
        Character* character = party.getMember(i);
        if (character != nullptr && character->isAlive())
        {
            character->addStress(stressAmount);
        }
    }

    printRule('-');
    std::cout << CYAN << BOLD << "  ROOM EVENT" << RESET << '\n';
    printRule('-');
    triggerEvent(party, inventory, gold);

    printRule('-');
    std::cout << GOLD << BOLD << "  PARTY STATUS" << RESET << '\n';
    printRule('-');

    for (int i = 1; i <= 4; ++i)
    {
        Character* character = party.getMember(i);
        if (character == nullptr)
        {
            continue;
        }

        std::cout << "  [" << i << "] " << WHITE << character->getName() << RESET
                  << "  HP " << healthColor(*character) << character->getHP()
                  << '/' << character->getMaxHP() << RESET
                  << "  Stress " << stressColor(*character)
                  << character->getStress() << "/200" << RESET;

        if (!character->isAlive())
        {
            std::cout << ' ' << RED << BOLD << "FALLEN" << RESET;
        }
        std::cout << '\n';
    }

    printRule('=');

    if (type != RoomType::Entrance && type != RoomType::Exit && battleOccurs())
    {
        std::cout << RED << BOLD
                  << "  The room stirs... an enemy party appears!\n"
                  << RESET;
        Input::waitForEnter();

        Battle battle(party);
        battle.start();
    }
    else
    {
        std::cout << GREEN << "  The room remains quiet.\n" << RESET;
        Input::waitForEnter();
    }
}

void Room::triggerEvent(Party& party, Inventory* inventory, int* gold)
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
        triggerChest(party, inventory, gold);
        break;
    case RoomType::Fountain:
        triggerFountain(party);
        break;
    case RoomType::Entrance:
    case RoomType::Exit:
        break;
    }
}

void Room::triggerEmpty()
{
    std::cout << CYAN << "  Nothing of interest is found.\n" << RESET;
}

void Room::triggerTrap(Party& party)
{
    std::cout << RED << BOLD << "  A trap has been triggered!\n" << RESET;

    // Preserve the existing trap effects: +10 stress to the living party,
    // followed by random damage (2-5) and another +5 stress to one hero.
    for (int i = 1; i <= 4; ++i)
    {
        Character* character = party.getMember(i);
        if (character != nullptr && character->isAlive())
        {
            character->addStress(10);
        }
    }

    std::cout << "  The party gains " << RED << "+10 stress each" << RESET << ".\n";

    Character* targets[4];
    int targetCount = 0;
    for (int i = 1; i <= 4; ++i)
    {
        Character* character = party.getMember(i);
        if (character != nullptr && character->isAlive())
        {
            targets[targetCount++] = character;
        }
    }

    if (targetCount == 0)
    {
        return;
    }

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<int> targetDistribution(0, targetCount - 1);
    std::uniform_int_distribution<int> damageDistribution(2, 5);

    Character* target = targets[targetDistribution(generator)];
    const int damage = damageDistribution(generator);
    target->takeDamage(damage);
    target->addStress(5);

    std::cout << "  " << target->getName() << " takes "
              << RED << BOLD << damage << " damage" << RESET
              << " and gains " << RED << "+5 stress" << RESET << ".\n";
}

void Room::triggerChest(Party& party, Inventory* inventory, int* gold)
{
    (void)party;
    std::cout << GOLD << BOLD << "  A treasure chest has been discovered!\n" << RESET;

    if (gold == nullptr)
    {
        std::cout << DIM << "  The chest is not connected to expedition supplies.\n" << RESET;
        return;
    }

    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<int> rewardDistribution(15, 35);
    const int reward = rewardDistribution(generator);

    *gold += reward;
    std::cout << "  You find " << GOLD << BOLD << reward << " gold" << RESET << ".\n";
    std::cout << "  Gold pouch: " << GOLD << BOLD << *gold << RESET << " gold.\n";
    (void)inventory;
}

void Room::triggerFountain(Party& party)
{
    std::cout << GREEN << BOLD << "  An old fountain glimmers in the gloom.\n" << RESET;

    for (int i = 1; i <= 4; ++i)
    {
        Character* character = party.getMember(i);
        if (character != nullptr && character->isAlive())
        {
            character->reduceStress(10);
        }
    }

    std::cout << "  Living heroes lose " << GREEN << "10 stress" << RESET << ".\n";

    bool hasInjuredCharacter = false;
    for (int i = 1; i <= 4; ++i)
    {
        Character* character = party.getMember(i);
        if (character != nullptr && character->isAlive() &&
            character->getHP() < character->getMaxHP())
        {
            hasInjuredCharacter = true;
            break;
        }
    }

    if (!hasInjuredCharacter)
    {
        std::cout << GREEN << "  Everyone is at full health.\n" << RESET;
        return;
    }

    printRule('-');
    std::cout << CYAN << BOLD << "  FOUNTAIN HEALING" << RESET << '\n';
    std::cout << DIM << "  Choose an injured hero to restore 3 HP.\n" << RESET;

    for (int i = 1; i <= 4; ++i)
    {
        Character* character = party.getMember(i);
        if (character != nullptr && character->isAlive())
        {
            std::cout << "  " << GOLD << '[' << i << "] " << RESET
                      << character->getName() << "  HP: "
                      << healthColor(*character) << character->getHP()
                      << '/' << character->getMaxHP() << RESET << '\n';
        }
    }
    std::cout << "  " << DIM << "[0] Back" << RESET << '\n';
    printRule('-');

    int choice = Input::getInt("  Choose a hero: ");
    if (choice == 0)
    {
        return;
    }

    Character* target = party.getMember(choice);
    while (target == nullptr ||
           !target->isAlive() ||
           target->getHP() >= target->getMaxHP())
    {
        std::cout << RED << "  Invalid target. Select an injured, living hero.\n" << RESET;
        choice = Input::getInt("  Choose a hero (0 to cancel): ");
        if (choice == 0)
        {
            return;
        }
        target = party.getMember(choice);
    }

    target->heal(3);
    std::cout << GREEN << "  " << target->getName() << " is healed for 3 HP.\n" << RESET;
}

RoomType Room::getType() const
{
    return type;
}
