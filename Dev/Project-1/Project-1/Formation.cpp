#include "Formation.h"

#include <iostream>

Formation::Formation()
{
    for (int i = 0; i < 4; i++)
    {
        members[i] = nullptr;
    }
}

bool Formation::addMember(Character& character, int position)
{
    if (position < 1 || position > 4)
    {
        return false;
    }

    // Don't place the same character twice.
    for (int i = 0; i < 4; i++)
    {
        if (members[i] == &character)
        {
            return false;
        }
    }

    // Don't overwrite an occupied position.
    if (members[position - 1] != nullptr)
    {
        return false;
    }

    members[position - 1] = &character;
    character.setPosition(position);

    return true;
}

bool Formation::removeMember(int position)
{
    if (position < 1 || position > 4)
    {
        return false;
    }

    Character* character = members[position - 1];
    if (character == nullptr)
    {
        return false;
    }

    members[position - 1] = nullptr;
    character->setPosition(0);
    return true;
}

Character* Formation::getMember(int position) const
{
    if (position < 1 || position > 4)
    {
        return nullptr;
    }

    return members[position - 1];
}

bool Formation::moveMember(int fromPosition, int toPosition)
{
    if (fromPosition < 1 || fromPosition > 4 ||
        toPosition < 1 || toPosition > 4)
    {
        return false;
    }

    if (fromPosition == toPosition)
    {
        return members[fromPosition - 1] != nullptr;
    }

    Character* movingCharacter = members[fromPosition - 1];

    if (movingCharacter == nullptr)
    {
        return false;
    }

    Character* destinationCharacter = members[toPosition - 1];

    // Move or swap the characters.
    members[toPosition - 1] = movingCharacter;
    members[fromPosition - 1] = destinationCharacter;

    movingCharacter->setPosition(toPosition);

    if (destinationCharacter != nullptr)
    {
        destinationCharacter->setPosition(fromPosition);
    }

    return true;
}

int Formation::getPosition(const Character& character) const
{
    for (int i = 0; i < 4; i++)
    {
        if (members[i] == &character)
        {
            return i + 1;
        }
    }

    // Zero means the character isn't in this formation.
    return 0;
}

void Formation::printFormation() const
{
    for (int i = 0; i < 4; i++)
    {
        std::cout << i + 1 << ". ";

        if (members[i] != nullptr)
        {
            std::cout << members[i]->getName();
        }
        else
        {
            std::cout << "Empty";
        }

        std::cout << "\n";
    }
}

bool Formation::isValid() const
{
    for (int i = 0; i < 4; i++)
    {
        if (members[i] == nullptr)
        {
            return false;
        }
    }

    return true;
}