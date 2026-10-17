#pragma once

#include "Inventory.h"
#include "Party.h"
#include "Room.h"

#include <memory>
#include <vector>

// Owns persistent campaign state. Party and Battle keep non-owning pointers
// to heroes; this roster owns every hero for the life of the campaign.
class Campaign
{
private:
    Party party;
    Inventory inventory;
    int gold;
    LightLevel lightLevel;
    std::vector<std::shared_ptr<Character> > roster;
    bool stageCoachUsed = false;

public:
    Campaign();
    ~Campaign();

    Campaign(const Campaign&) = delete;
    Campaign& operator=(const Campaign&) = delete;

    Party& getParty();
    Inventory& getInventory();
    LightLevel& getLightLevel();
    LightLevel getLightLevel() const;

    int getGold() const;
    int& getGold();
    bool spendGold(int amount);
    void addGold(int amount);

    int getRosterSize() const;
    Character* getRosterMember(int rosterIndex) const; // One-based index.
    bool isInParty(const Character& character) const;
    Character* addHero(std::shared_ptr<Character> hero);
    bool dismissHero(Character* hero);
    bool hasLivingPartyMember() const;
    bool hasUsedStageCoach() const;
    void markStageCoachUsed();
   
};
