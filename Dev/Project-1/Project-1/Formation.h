#pragma once

#include "Character.h"

class Formation
{
private:
    Character* members[4];

public:
    Formation();

    bool addMember(Character& character, int position);
    Character* getMember(int position) const;

    bool moveMember(int fromPosition, int toPosition);
    int getPosition(const Character& character) const;

    void printFormation() const;
    bool isValid() const;
};