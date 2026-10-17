#include "Campaign.h"

#include "Crusader.h"
#include "Highwayman.h"

#include <utility>

Campaign::Campaign()
    : inventory(5, 3, 2),
      gold(100),
      lightLevel(LightLevel::Bright)
{
    // The campaign starts with exactly two heroes: Crusader and Highwayman.
    Character* crusader = addHero(std::shared_ptr<Character>(new Crusader()));
    Character* highwayman = addHero(std::shared_ptr<Character>(new Highwayman()));

    if (crusader != nullptr)
    {
        party.addMember(*crusader, 1);
    }

    if (highwayman != nullptr)
    {
        party.addMember(*highwayman, 2);
    }
}

Campaign::~Campaign() = default;

Party& Campaign::getParty()
{
    return party;
}

Inventory& Campaign::getInventory()
{
    return inventory;
}

LightLevel& Campaign::getLightLevel()
{
    return lightLevel;
}

LightLevel Campaign::getLightLevel() const
{
    return lightLevel;
}

int Campaign::getGold() const
{
    return gold;
}

int& Campaign::getGold()
{
    return gold;
}

bool Campaign::spendGold(int amount)
{
    if (amount < 0 || gold < amount)
    {
        return false;
    }

    gold -= amount;
    return true;
}

void Campaign::addGold(int amount)
{
    if (amount > 0)
    {
        gold += amount;
    }
}

int Campaign::getRosterSize() const
{
    return static_cast<int>(roster.size());
}

Character* Campaign::getRosterMember(int rosterIndex) const
{
    if (rosterIndex < 1 || rosterIndex > static_cast<int>(roster.size()))
    {
        return nullptr;
    }

    return roster[rosterIndex - 1].get();
}

bool Campaign::isInParty(const Character& character) const
{
    return party.getPosition(character) != 0;
}

Character* Campaign::addHero(std::shared_ptr<Character> hero)
{
    if (!hero)
    {
        return nullptr;
    }

    Character* storedHero = hero.get();
    roster.push_back(std::move(hero));
    return storedHero;
}

bool Campaign::dismissHero(Character* hero)
{
    if (hero == nullptr)
    {
        return false;
    }

    for (std::vector<std::shared_ptr<Character> >::iterator it = roster.begin();
         it != roster.end(); ++it)
    {
        if (it->get() == hero)
        {
            const int position = party.getPosition(*hero);
            if (position != 0)
            {
                party.removeMember(position);
            }

            roster.erase(it);
            return true;
        }
    }

    return false;
}

bool Campaign::hasLivingPartyMember() const
{
    for (int position = 1; position <= 4; ++position)
    {
        Character* character = party.getMember(position);
        if (character != nullptr && character->isAlive())
        {
            return true;
        }
    }

    return false;
}
