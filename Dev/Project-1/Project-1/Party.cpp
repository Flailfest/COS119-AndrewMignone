#include "Party.h"

#include <iostream>

// --------------------------------------------------
// Constructor
// --------------------------------------------------

Party::Party()
{
    // Start with all four positions empty.
    for (int i = 0; i < 4; i++)
    {
        members[i] = nullptr;
    }
}

// --------------------------------------------------
// Add Member
// --------------------------------------------------

bool Party::addMember(
    Character& character,
    int position
)
{
    // Positions must be between 1 and 4.
    if (position < 1 || position > 4)
    {
        std::cout
            << "Invalid position. "
            << "Party positions must be between 1 and 4.\n";

        return false;
    }

    // Convert the player's position (1-4)
    // into the array index (0-3).
    int index = position - 1;

    // Check whether the position is already occupied.
    if (members[index] != nullptr)
    {
        std::cout
            << "Position "
            << position
            << " is already occupied by "
            << members[index]->getName()
            << ".\n";

        return false;
    }

    // Add the character to the party.
    members[index] = &character;

    // Tell the character which position
    // they occupy in the party.
    character.setPosition(position);

    std::cout
        << character.getName()
        << " joined the party in position "
        << position
        << ".\n";

    return true;
}

// --------------------------------------------------
// Get Member
// --------------------------------------------------

Character* Party::getMember(int position) const
{
    // Invalid positions return nullptr.
    if (position < 1 || position > 4)
    {
        return nullptr;
    }

    // Convert position 1-4 into index 0-3.
    return members[position - 1];
}

// --------------------------------------------------
// Print Formation
// --------------------------------------------------

void Party::printFormation() const
{
    std::cout << "\n";
    std::cout << "========== PARTY ==========\n";

    for (int i = 0; i < 4; i++)
    {
        std::cout
            << "[" << i + 1 << "] ";

        if (members[i] != nullptr)
        {
            std::cout
                << members[i]->getName();
        }
        else
        {
            std::cout
                << "--- EMPTY ---";
        }

        std::cout << "\n";
    }

    std::cout << "===========================\n";
}

// --------------------------------------------------
// Check Party Formation
// --------------------------------------------------

bool Party::isValid() const
{
    // A valid party currently requires
    // all four positions to be occupied.

    for (int i = 0; i < 4; i++)
    {
        if (members[i] == nullptr)
        {
            return false;
        }
    }

    return true;
}