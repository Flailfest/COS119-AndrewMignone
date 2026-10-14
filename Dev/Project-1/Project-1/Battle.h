#pragma once

#include "Party.h"
#include "Enemy.h"

#include <string>

class Battle
{
private:
    Party& playerParty;
    Party enemyParty;

    Enemy* enemyRoster[4];

    int enemyCount;

    void generateEnemyParty();
    Enemy* createRandomEnemy();

    // Opens the formation movement menu.
// Returns true if a move succeeds, false if cancelled.
    bool movePartyMember();

    // Stores the most recent combat action for the next screen redraw.
    std::string lastActionMessage;

    void printBattleState() const;
    void printCharacterStatus(const Character& character) const;

    void playerTurn(Character& character);
    void enemyTurn(Character& enemy);

    bool attackMenu(Character& character);
    void skillMenu(Character& character);
    Character* chooseEnemyTarget(Character& character, int minimumPosition, int maximumPosition);
    Character* chooseAllyTarget();

    bool playerPartyAlive() const;
    bool enemyPartyAlive() const;

public:
    Battle(Party& playerParty);
    ~Battle();

    void start();

    Party& getPlayerParty();
    Party& getEnemyParty();

};