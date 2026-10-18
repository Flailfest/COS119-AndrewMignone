#include "Party.h"

#include <iostream>

Party::Party() = default;

bool Party::addMember(Character& character, int position)
{
    if (!formation.addMember(character, position))
    {
        std::cout
            << "Could not add "
            << character.getName()
            << " at position "
            << position
            << ".\n";

        return false;
    }

    std::cout
        << character.getName()
        << " joined the party at position "
        << position
        << ".\n";

    return true;
}

bool Party::removeMember(int position)
{
    return formation.removeMember(position);
}

Character* Party::getMember(int position) const
{
    return formation.getMember(position);
}

bool Party::moveMember(int fromPosition, int toPosition)
{
    return formation.moveMember(fromPosition, toPosition);
}

int Party::getPosition(const Character& character) const
{
    return formation.getPosition(character);
}

void Party::printFormation() const
{
    formation.printFormation();
}

bool Party::isValid() const
{
    return formation.isValid();
}