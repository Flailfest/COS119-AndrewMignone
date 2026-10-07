#pragma once

class Inventory
{
private:
    int food;
    int torches;
    int medicine;

public:
    Inventory(
        int startingFood = 0,
        int startingTorches = 0,
        int startingMedicine = 0
    );

    int getFood() const;
    int getTorches() const;
    int getMedicine() const;

    void addFood(int amount);
    void addTorches(int amount);
    void addMedicine(int amount);

    bool useFood();
    bool useTorch();
    bool useMedicine();

    void printInventory() const;
};