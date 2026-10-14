#pragma once

#include "Formation.h"

class Party
{
private:
    Formation formation;

public:
    Party();

    bool addMember(Character& character, int position);
    Character* getMember(int position) const;

    bool moveMember(int fromPosition, int toPosition);
    int getPosition(const Character& character) const;

    void printFormation() const;
    bool isValid() const;
};