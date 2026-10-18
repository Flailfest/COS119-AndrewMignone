#include "Town.h"

#include "Campaign.h"
#include "Character.h"
#include "Crusader.h"
#include "FloorTest.h"
#include "Highwayman.h"
#include "Input.h"
#include "PlagueDoctor.h"
#include "Vestal.h"
#include "Arbalest.h"
#include "Musketeer.h"
#include "ManAtArms.h"
#include "Hellion.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace
{
    const char* const RESET = "\033[0m";
    const char* const BOLD = "\033[1m";
    const char* const DIM = "\033[38;5;240m";
    const char* const WHITE = "\033[38;5;252m";
    const char* const RED = "\033[38;5;160m";
    const char* const GREEN = "\033[38;5;114m";
    const char* const CYAN = "\033[38;5;80m";
    const char* const GOLD = "\033[38;5;220m";
    const int UI_WIDTH = 62;

    void clearTownScreen()
    {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }

    void printRule(char character = '=', const char* color = DIM)
    {
        std::cout << color << std::string(UI_WIDTH, character) << RESET << '\n';
    }

    void printHeader(const std::string& title, const char* color = GOLD)
    {
        printRule('=', color);
        std::cout << color << BOLD << "  " << title << RESET << '\n';
        printRule('=', color);
        std::cout << '\n';
    }

    int randomInt(int minimum, int maximum)
    {
        static std::random_device device;
        static std::mt19937 generator(device());
        std::uniform_int_distribution<int> distribution(minimum, maximum);
        return distribution(generator);
    }

    std::shared_ptr<Character> createRandomHero()
    {
        switch (randomInt(1, 8))
        {
        case 1:
            return std::shared_ptr<Character>(new Crusader());
        case 2:
            return std::shared_ptr<Character>(new Highwayman());
        case 3:
            return std::shared_ptr<Character>(new PlagueDoctor());
        case 4:
            return std::shared_ptr<Character>(new Vestal());
        case 5:
            return std::shared_ptr<Character>(new Arbalest());
        case 6:
            return std::shared_ptr<Character>(new Musketeer());
        case 7:
            return std::shared_ptr<Character>(new ManAtArms());
        default:
            return std::shared_ptr<Character>(new Hellion());
        }
    }

    const char* lightLevelName(LightLevel level)
    {
        switch (level)
        {
        case LightLevel::Bright: return "Bright";
        case LightLevel::Dim: return "Dim";
        case LightLevel::Off: return "Off";
        }
        return "Unknown";
    }

    const char* lightLevelColor(LightLevel level)
    {
        switch (level)
        {
        case LightLevel::Bright: return GREEN;
        case LightLevel::Dim: return GOLD;
        case LightLevel::Off: return RED;
        }
        return WHITE;
    }

    const char* healthColor(const Character& hero)
    {
        if (hero.getMaxHP() <= 0 || hero.getHP() * 4 <= hero.getMaxHP())
        {
            return RED;
        }
        if (hero.getHP() * 2 <= hero.getMaxHP())
        {
            return GOLD;
        }
        return GREEN;
    }

    void printTownStatus(const Campaign& campaign)
    {
        std::cout << "  Gold       : " << GOLD << BOLD << campaign.getGold() << RESET << '\n';
        std::cout << "  Torchlight : " << lightLevelColor(campaign.getLightLevel())
                  << BOLD << lightLevelName(campaign.getLightLevel()) << RESET << '\n';
        std::cout << "  Roster     : " << CYAN << campaign.getRosterSize()
                  << " hero(es)" << RESET << '\n';
    }

    void showInventory(Campaign& campaign)
    {
        clearTownScreen();
        printHeader("CAMPAIGN SUPPLIES", GOLD);
        Inventory& inventory = campaign.getInventory();
        std::cout << "  Food       : " << GREEN << inventory.getFood() << RESET << '\n';
        std::cout << "  Torches    : " << GOLD << inventory.getTorches() << RESET << '\n';
        std::cout << "  Medicine   : " << CYAN << inventory.getMedicine() << RESET << '\n';
        std::cout << "  Gold       : " << GOLD << BOLD << campaign.getGold() << RESET << '\n';
        std::cout << "  Torchlight : " << lightLevelColor(campaign.getLightLevel())
                  << BOLD << lightLevelName(campaign.getLightLevel()) << RESET << "\n\n";
        printRule();
        Input::waitForEnter();
    }

    void supplyShop(Campaign& campaign)
    {
        const int FOOD_PRICE = 10;
        const int TORCH_PRICE = 15;
        const int MEDICINE_PRICE = 20;

        while (true)
        {
            clearTownScreen();
            printHeader("SUPPLY SHOP", GOLD);
            printTownStatus(campaign);
            printRule('-');
            std::cout << CYAN << BOLD << "  AVAILABLE SUPPLIES" << RESET << '\n';
            std::cout << "  " << GOLD << "1" << RESET << ". Food       - " << FOOD_PRICE << " gold each\n";
            std::cout << "  " << GOLD << "2" << RESET << ". Torch      - " << TORCH_PRICE << " gold each\n";
            std::cout << "  " << GOLD << "3" << RESET << ". Medicine   - " << MEDICINE_PRICE << " gold each\n";
            std::cout << "  " << DIM << "0. Back" << RESET << '\n';
            printRule();

            const int choice = Input::getIntInRange("\nChoose a supply: ", 0, 3);
            if (choice == 0)
            {
                return;
            }

            const int price = choice == 1 ? FOOD_PRICE :
                              choice == 2 ? TORCH_PRICE : MEDICINE_PRICE;
            const int maxAffordable = campaign.getGold() / price;
            if (maxAffordable <= 0)
            {
                std::cout << RED << "\nYou cannot afford this supply.\n" << RESET;
                Input::waitForEnter();
                continue;
            }

            const int quantity = Input::getIntInRange(
                "How many? (1-" + std::to_string(maxAffordable) + ", 0 cancels): ",
                0,
                maxAffordable
            );
            if (quantity == 0)
            {
                continue;
            }

            const int totalCost = quantity * price;
            if (!campaign.spendGold(totalCost))
            {
                std::cout << RED << "Purchase failed; gold was unchanged.\n" << RESET;
                Input::waitForEnter();
                continue;
            }

            if (choice == 1)
            {
                campaign.getInventory().addFood(quantity);
            }
            else if (choice == 2)
            {
                campaign.getInventory().addTorches(quantity);
            }
            else
            {
                campaign.getInventory().addMedicine(quantity);
            }

            std::cout << GREEN << "\nPurchase complete: " << quantity
                      << " item(s) for " << totalCost << " gold.\n" << RESET;
            Input::waitForEnter();
        }
    }

    void stressRelief(Campaign& campaign)
    {
        const int COST = 40;
        const int REDUCTION = 20;

        while (true)
        {
            clearTownScreen();
            printHeader("STRESS RELIEF", CYAN);
            std::cout << "  Cost: " << GOLD << COST << " gold" << RESET
                      << " per visit. Reduce stress by " << GREEN << REDUCTION
                      << RESET << ".\n\n";
            std::cout << "  Available gold: " << GOLD << BOLD << campaign.getGold() << RESET << '\n';
            printRule('-');

            for (int position = 1; position <= 4; ++position)
            {
                Character* hero = campaign.getParty().getMember(position);
                if (hero == nullptr)
                {
                    std::cout << DIM << "  [" << position << "] Empty slot\n" << RESET;
                    continue;
                }

                std::cout << "  " << GOLD << '[' << position << "] " << RESET
                          << hero->getName() << "  Stress: "
                          << (hero->getStress() >= 100 ? RED : CYAN)
                          << hero->getStress() << "/200" << RESET;
                if (!hero->isAlive())
                {
                    std::cout << ' ' << RED << "FALLEN" << RESET;
                }
                std::cout << '\n';
            }

            std::cout << "  " << DIM << "[0] Back" << RESET << '\n';
            printRule();
            const int choice = Input::getIntInRange("Choose a party member: ", 0, 4);
            if (choice == 0)
            {
                return;
            }

            Character* hero = campaign.getParty().getMember(choice);
            if (hero == nullptr)
            {
                std::cout << RED << "That formation slot is empty.\n" << RESET;
                Input::waitForEnter();
                continue;
            }

            if (hero->getStress() <= 0)
            {
                std::cout << GREEN << hero->getName() << " has no stress to relieve.\n" << RESET;
                Input::waitForEnter();
                continue;
            }

            if (!campaign.spendGold(COST))
            {
                std::cout << RED << "You need " << COST << " gold for stress relief.\n" << RESET;
                Input::waitForEnter();
                continue;
            }

            const int before = hero->getStress();
            hero->reduceStress(REDUCTION);
            std::cout << GREEN << "\n" << hero->getName() << "'s stress falls from "
                      << before << " to " << hero->getStress() << ".\n" << RESET;
            Input::waitForEnter();
        }
    }

    void assignNewRecruitIfWanted(Campaign& campaign, Character* hero)
    {
        std::vector<int> emptySlots;
        for (int position = 1; position <= 4; ++position)
        {
            if (campaign.getParty().getMember(position) == nullptr)
            {
                emptySlots.push_back(position);
            }
        }

        if (emptySlots.empty())
        {
            std::cout << DIM << "  Your active party is full; the recruit remains in the roster.\n" << RESET;
            return;
        }

        std::cout << "\n  Assign " << CYAN << hero->getName() << RESET
                  << " to an open formation slot?\n";
        for (std::size_t i = 0; i < emptySlots.size(); ++i)
        {
            std::cout << "  " << GOLD << '[' << emptySlots[i] << "] " << RESET
                      << "Position " << emptySlots[i] << '\n';
        }
        std::cout << "  " << DIM << "[0] Keep as reserve" << RESET << '\n';

        while (true)
        {
            const int position = Input::getIntInRange("Choose a position: ", 0, 4);
            if (position == 0)
            {
                return;
            }
            if (campaign.getParty().getMember(position) != nullptr)
            {
                std::cout << RED << "That position is occupied. Choose an empty slot.\n" << RESET;
                continue;
            }
            if (campaign.getParty().addMember(*hero, position))
            {
                std::cout << GREEN << hero->getName() << " joins the active party.\n" << RESET;
            }
            return;
        }
    }

    void stageCoach(Campaign& campaign)
    {
        // Prevent the Stage Coach from being used twice.
        if (campaign.hasUsedStageCoach())
        {
            clearTownScreen();
            printHeader("STAGE COACH", GOLD);

            std::cout << RED
                << "  The stage coach has already departed.\n"
                << RESET;

            Input::waitForEnter();
            return;
        }

        // Mark it as used before generating recruits.
        campaign.markStageCoachUsed();

        const int arrivalCount = randomInt(2, 4);
        std::vector<std::shared_ptr<Character> > arrivals;
        for (int i = 0; i < arrivalCount; ++i)
        {
            arrivals.push_back(createRandomHero());
        }

        while (!arrivals.empty())
        {
            clearTownScreen();
            printHeader("STAGE COACH", GOLD);
            std::cout << "  " << arrivals.size() << " recruit(s) are waiting."
                      << " Choose who to hire; unselected arrivals will leave.\n\n";
            printRule('-');

            for (std::size_t i = 0; i < arrivals.size(); ++i)
            {
                Character* hero = arrivals[i].get();
                std::cout << "  " << GOLD << '[' << (i + 1) << "] " << RESET
                          << CYAN << BOLD << hero->getName() << RESET
                          << "  HP " << hero->getMaxHP()
                          << "  Stress " << hero->getStress() << "/200\n";
            }
            std::cout << "  " << DIM << "[0] Finish recruiting" << RESET << '\n';
            printRule();

            const int choice = Input::getIntInRange(
                "Choose a recruit: ", 0, static_cast<int>(arrivals.size())
            );
            if (choice == 0)
            {
                break;
            }

            std::shared_ptr<Character> selected = arrivals[choice - 1];
            arrivals.erase(arrivals.begin() + (choice - 1));
            Character* recruited = campaign.addHero(selected);
            if (recruited == nullptr)
            {
                continue;
            }

            std::cout << GREEN << "\n" << recruited->getName()
                      << " has joined your roster.\n" << RESET;
            assignNewRecruitIfWanted(campaign, recruited);
            Input::waitForEnter();
        }

        clearTownScreen();
        printHeader("STAGE COACH", GOLD);
        std::cout << "  The stage coach has departed.\n\n";
        Input::waitForEnter();
    }

    void printPartyAndRoster(Campaign& campaign)
    {
        clearTownScreen();
        printHeader("PARTY & ROSTER", CYAN);
        std::cout << CYAN << BOLD << "  ACTIVE FORMATION" << RESET << '\n';
        printRule('-');

        for (int position = 1; position <= 4; ++position)
        {
            Character* hero = campaign.getParty().getMember(position);
            std::cout << "  " << GOLD << '[' << position << "] " << RESET;
            if (hero == nullptr)
            {
                std::cout << DIM << "Empty" << RESET << '\n';
            }
            else
            {
                std::cout << hero->getName() << "  HP " << healthColor(*hero)
                          << hero->getHP() << '/' << hero->getMaxHP() << RESET
                          << "  Stress " << (hero->getStress() >= 100 ? RED : CYAN)
                          << hero->getStress() << "/200" << RESET;
                if (!hero->isAlive())
                {
                    std::cout << ' ' << RED << "FALLEN" << RESET;
                }
                std::cout << '\n';
            }
        }

        std::cout << '\n' << CYAN << BOLD << "  FULL ROSTER" << RESET << '\n';
        printRule('-');
        for (int index = 1; index <= campaign.getRosterSize(); ++index)
        {
            Character* hero = campaign.getRosterMember(index);
            const int position = campaign.getParty().getPosition(*hero);
            std::cout << "  " << GOLD << '[' << index << "] " << RESET
                      << hero->getName() << "  ";
            if (position != 0)
            {
                std::cout << GREEN << "Active: position " << position << RESET;
            }
            else
            {
                std::cout << DIM << "Reserve" << RESET;
            }
            std::cout << "  HP " << healthColor(*hero) << hero->getHP()
                      << '/' << hero->getMaxHP() << RESET
                      << "  Stress " << (hero->getStress() >= 100 ? RED : CYAN)
                      << hero->getStress() << "/200" << RESET << '\n';
        }
        if (campaign.getRosterSize() == 0)
        {
            std::cout << DIM << "  The roster is empty. Visit the Stage Coach to recruit heroes.\n" << RESET;
        }
        printRule();
        Input::waitForEnter();
    }

    void partyManagement(Campaign& campaign)
    {
        while (true)
        {
            clearTownScreen();
            printHeader("PARTY MANAGEMENT", CYAN);
            campaign.getParty().printFormation();
            std::cout << '\n';
            std::cout << "  " << GOLD << "1" << RESET << ". Assign a reserve hero to an empty slot\n";
            std::cout << "  " << GOLD << "2" << RESET << ". Remove a hero from the active party\n";
            std::cout << "  " << RED << "3" << RESET << ". Dismiss a hero permanently\n";
            std::cout << "  " << GOLD << "4" << RESET << ". View party and full roster\n";
            std::cout << "  " << DIM << "0. Back" << RESET << '\n';
            printRule();

            const int choice = Input::getIntInRange("Choose an option: ", 0, 4);
            if (choice == 0)
            {
                return;
            }

            if (choice == 1)
            {
                bool hasEmptySlot = false;
                for (int p = 1; p <= 4; ++p)
                {
                    if (campaign.getParty().getMember(p) == nullptr)
                    {
                        hasEmptySlot = true;
                        break;
                    }
                }
                if (!hasEmptySlot)
                {
                    std::cout << RED << "Your formation is full. Remove a hero first.\n" << RESET;
                    Input::waitForEnter();
                    continue;
                }

                std::vector<int> availableRosterIndices;
                std::cout << '\n' << CYAN << BOLD << "  RESERVE HEROES" << RESET << '\n';
                for (int index = 1; index <= campaign.getRosterSize(); ++index)
                {
                    Character* hero = campaign.getRosterMember(index);
                    if (!campaign.isInParty(*hero))
                    {
                        availableRosterIndices.push_back(index);
                        std::cout << "  " << GOLD << '[' << index << "] " << RESET
                                  << hero->getName() << "  HP " << hero->getHP()
                                  << '/' << hero->getMaxHP() << '\n';
                    }
                }
                if (availableRosterIndices.empty())
                {
                    std::cout << DIM << "  No reserve heroes are available. Visit the Stage Coach.\n" << RESET;
                    Input::waitForEnter();
                    continue;
                }

                const int rosterChoice = Input::getIntInRange(
                    "Choose a reserve hero (0 to cancel): ", 0, campaign.getRosterSize()
                );
                if (rosterChoice == 0)
                {
                    continue;
                }
                Character* hero = campaign.getRosterMember(rosterChoice);
                if (hero == nullptr || campaign.isInParty(*hero))
                {
                    std::cout << RED << "That hero is not in reserve.\n" << RESET;
                    Input::waitForEnter();
                    continue;
                }

                std::cout << '\n' << "Empty positions: ";
                for (int p = 1; p <= 4; ++p)
                {
                    if (campaign.getParty().getMember(p) == nullptr)
                    {
                        std::cout << '[' << p << "] ";
                    }
                }
                const int position = Input::getIntInRange(
                    "\nChoose an empty position (0 to cancel): ", 0, 4
                );
                if (position == 0)
                {
                    continue;
                }
                if (campaign.getParty().getMember(position) != nullptr)
                {
                    std::cout << RED << "That position is occupied.\n" << RESET;
                    Input::waitForEnter();
                    continue;
                }
                campaign.getParty().addMember(*hero, position);
                Input::waitForEnter();
                continue;
            }

            if (choice == 2)
            {
                const int position = Input::getIntInRange(
                    "Choose an occupied position to remove (0 to cancel): ", 0, 4
                );
                if (position == 0)
                {
                    continue;
                }
                Character* hero = campaign.getParty().getMember(position);
                if (hero == nullptr)
                {
                    std::cout << RED << "That position is empty.\n" << RESET;
                }
                else if (campaign.getParty().removeMember(position))
                {
                    std::cout << GREEN << hero->getName()
                              << " is now in reserve; they have not been dismissed.\n" << RESET;
                }
                Input::waitForEnter();
                continue;
            }

            if (choice == 3)
            {
                if (campaign.getRosterSize() == 0)
                {
                    std::cout << DIM << "Your roster is empty.\n" << RESET;
                    Input::waitForEnter();
                    continue;
                }

                std::cout << '\n' << RED << BOLD << "  DISMISS HERO" << RESET << '\n';
                for (int index = 1; index <= campaign.getRosterSize(); ++index)
                {
                    Character* hero = campaign.getRosterMember(index);
                    std::cout << "  " << GOLD << '[' << index << "] " << RESET
                              << hero->getName();
                    const int position = campaign.getParty().getPosition(*hero);
                    if (position != 0)
                    {
                        std::cout << " (active position " << position << ')';
                    }
                    std::cout << '\n';
                }
                std::cout << "  Dismissing permanently removes the hero and their progress.\n";
                const int index = Input::getIntInRange(
                    "Choose a hero to dismiss (0 to cancel): ", 0, campaign.getRosterSize()
                );
                if (index == 0)
                {
                    continue;
                }

                Character* hero = campaign.getRosterMember(index);
                const std::string name = hero != nullptr ? hero->getName() : "Hero";
                const int confirm = Input::getIntInRange(
                    "Permanently dismiss " + name + "? [1] Yes [0] No: ", 0, 1
                );
                if (confirm == 1 && campaign.dismissHero(hero))
                {
                    std::cout << GREEN << name << " has left the roster permanently.\n" << RESET;
                }
                Input::waitForEnter();
                continue;
            }

            if (choice == 4)
            {
                printPartyAndRoster(campaign);
            }
        }
    }
}

void Town::run(Campaign& campaign)
{
    while (true)
    {
        clearTownScreen();
        printHeader("THE HAMLET", GOLD);
        std::cout << DIM << "  The road back is cold. For now, the cellar can wait.\n\n" << RESET;
        printTownStatus(campaign);
        printRule('-');
        std::cout << CYAN << BOLD << "  TOWN SERVICES" << RESET << '\n';
        std::cout << "  " << GOLD << "1" << RESET << ". Enter the Cellar (start a floor run)\n";
        std::cout << "  " << GOLD << "2" << RESET << ". Supply Shop\n";
        std::cout << "  " << GOLD << "3" << RESET << ". Stress Relief\n";
        std::cout << "  " << GOLD << "4" << RESET
            << ". Stage Coach";

        if (campaign.hasUsedStageCoach())
        {
            std::cout << " " << RED << "[USED]" << RESET;
        }

        std::cout << "\n";
        std::cout << "  " << GOLD << "5" << RESET << ". Party Management\n";
        std::cout << "  " << GOLD << "6" << RESET << ". View Inventory\n";
        std::cout << "  " << DIM << "0. Return to Main Menu" << RESET << '\n';
        printRule();

        const int choice = Input::getIntInRange("\nChoose a service: ", 0, 6);
        if (choice == 0)
        {
            return;
        }

        if (choice == 1)
        {
            if (!campaign.hasLivingPartyMember())
            {
                std::cout << RED << "\nNo living heroes are in the active party.\n"
                          << "Use Party Management or the Stage Coach before entering the cellar.\n"
                          << RESET;
                Input::waitForEnter();
                continue;
            }

            FloorTest floorTest;
            floorTest.run(
                campaign.getParty(),
                campaign.getInventory(),
                campaign.getGold(),
                campaign.getLightLevel()
            );
            // FloorTest returns here after clearing a floor, defeat, or leaving.
            continue;
        }

        if (choice == 2)
        {
            supplyShop(campaign);
        }
        else if (choice == 3)
        {
            stressRelief(campaign);
        }
        else if (choice == 4)
        {
            stageCoach(campaign);
        }
        else if (choice == 5)
        {
            partyManagement(campaign);
        }
        else if (choice == 6)
        {
            showInventory(campaign);
        }
    }
}
