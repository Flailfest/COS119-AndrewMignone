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
    // Stores the narrator's current line.
    // It remains unchanged while the same turn's menu is open.
    std::string narratorMessage;

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

    // Checks whether high stress triggers a reaction.
    // Returns true if the reaction consumes the hero's turn.
    bool resolveStressReaction(Character& character);

public:
    Battle(Party& playerParty);
    ~Battle();

    void start();

    Party& getPlayerParty();
    Party& getEnemyParty();

};