#include "Inventory.h"

#include <iostream>

Inventory::Inventory(
    int startingFood,
    int startingTorches,
    int startingMedicine
)
    : food(startingFood),
    torches(startingTorches),
    medicine(startingMedicine)
{
}

int Inventory::getFood() const
{
    return food;
}

int Inventory::getTorches() const
{
    return torches;
}

int Inventory::getMedicine() const
{
    return medicine;
}

void Inventory::addFood(int amount)
{
    if (amount > 0)
    {
        food += amount;
    }
}

void Inventory::addTorches(int amount)
{
    if (amount > 0)
    {
        torches += amount;
    }
}

void Inventory::addMedicine(int amount)
{
    if (amount > 0)
    {
        medicine += amount;
    }
}

bool Inventory::useFood()
{
    if (food <= 0)
    {
        return false;
    }

    food--;
    return true;
}

bool Inventory::useTorch()
{
    if (torches <= 0)
    {
        return false;
    }

    torches--;
    return true;
}

bool Inventory::useMedicine()
{
    if (medicine <= 0)
    {
        return false;
    }

    medicine--;
    return true;
}

void Inventory::printInventory() const
{
    std::cout << "\n";
    std::cout << "====================================\n";
    std::cout << "             INVENTORY\n";
    std::cout << "====================================\n\n";

    std::cout << "Food:       " << food << "\n";
    std::cout << "Torches:    " << torches << "\n";
    std::cout << "Medicine:   " << medicine << "\n";
}