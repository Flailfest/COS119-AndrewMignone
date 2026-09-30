#pragma once

#include "Party.h"
#include "Enemy.h"

class Battle
{
private:
    Party& playerParty;
    Party enemyParty;

    Enemy* enemyRoster[4];

    int enemyCount;

    void generateEnemyParty();
    Enemy* createRandomEnemy();

    void printBattleState() const;
    void printCharacterStatus(const Character& character) const;

    void playerTurn(Character& character);
    void enemyTurn(Character& enemy);

    void attackMenu(Character& character);
    void skillMenu(Character& character);

    Character* chooseEnemyTarget();
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