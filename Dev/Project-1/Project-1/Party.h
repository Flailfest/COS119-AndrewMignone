#pragma once

#include "Character.h"

class Party
{
private:
    Character* members[4];

public:
    Party();

    bool addMember(Character& character, int position);

    Character* getMember(int position) const;

    void printFormation() const;

    bool isValid() const;
};