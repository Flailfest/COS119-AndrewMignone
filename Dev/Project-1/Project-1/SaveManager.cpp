#include "SaveManager.h"

#include "Campaign.h"
#include "Character.h"
#include "Crusader.h"
#include "Highwayman.h"
#include "PlagueDoctor.h"
#include "Vestal.h"
#include "Arbalest.h"
#include "Musketeer.h"
#include "ManAtArms.h"
#include "Hellion.h"
#include "Leper.h"
#include "Houndmaster.h"
#include "Occultist.h"
#include "BountyHunter.h"
#include "GraveRobber.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

namespace
{
    const int SAVE_VERSION = 1;
    const std::size_t MAX_ROSTER_SIZE = 100;

    struct SavedHero
    {
        std::shared_ptr<Character> hero;
        int hp;
        int stress;
        int position;
    };

    const char* getHeroType(const Character& hero)
    {
        if (dynamic_cast<const Crusader*>(&hero)) return "Crusader";
        if (dynamic_cast<const Highwayman*>(&hero)) return "Highwayman";
        if (dynamic_cast<const PlagueDoctor*>(&hero)) return "PlagueDoctor";
        if (dynamic_cast<const Vestal*>(&hero)) return "Vestal";
        if (dynamic_cast<const Arbalest*>(&hero)) return "Arbalest";
        if (dynamic_cast<const Musketeer*>(&hero)) return "Musketeer";
        if (dynamic_cast<const ManAtArms*>(&hero)) return "ManAtArms";
        if (dynamic_cast<const Hellion*>(&hero)) return "Hellion";
        if (dynamic_cast<const Leper*>(&hero)) return "Leper";
        if (dynamic_cast<const Houndmaster*>(&hero)) return "Houndmaster";
        if (dynamic_cast<const Occultist*>(&hero)) return "Occultist";
        if (dynamic_cast<const BountyHunter*>(&hero)) return "BountyHunter";
        if (dynamic_cast<const GraveRobber*>(&hero)) return "GraveRobber";
        return nullptr;
    }

    std::shared_ptr<Character> createHero(const std::string& type)
    {
        if (type == "Crusader") return std::shared_ptr<Character>(new Crusader());
        if (type == "Highwayman") return std::shared_ptr<Character>(new Highwayman());
        if (type == "PlagueDoctor") return std::shared_ptr<Character>(new PlagueDoctor());
        if (type == "Vestal") return std::shared_ptr<Character>(new Vestal());
        if (type == "Arbalest") return std::shared_ptr<Character>(new Arbalest());
        if (type == "Musketeer") return std::shared_ptr<Character>(new Musketeer());
        if (type == "ManAtArms") return std::shared_ptr<Character>(new ManAtArms());
        if (type == "Hellion") return std::shared_ptr<Character>(new Hellion());
        if (type == "Leper") return std::shared_ptr<Character>(new Leper());
        if (type == "Houndmaster") return std::shared_ptr<Character>(new Houndmaster());
        if (type == "Occultist") return std::shared_ptr<Character>(new Occultist());
        if (type == "BountyHunter") return std::shared_ptr<Character>(new BountyHunter());
        if (type == "GraveRobber") return std::shared_ptr<Character>(new GraveRobber());
        return std::shared_ptr<Character>();
    }

    bool normalizeSaveName(const std::string& input, std::string& normalized)
    {
        normalized = input;
        if (normalized.size() >= 4 &&
            normalized.substr(normalized.size() - 4) == ".sav")
        {
            normalized.erase(normalized.size() - 4);
        }

        if (normalized.empty())
        {
            return false;
        }

        for (unsigned char character : normalized)
        {
            if (!std::isalnum(character) && character != '_' && character != '-')
            {
                return false;
            }
        }
        return true;
    }

    bool buildSavePath(const std::string& nameOrFile, std::string& path)
    {
        std::string name;
        if (!normalizeSaveName(nameOrFile, name))
        {
            return false;
        }
        path = std::string("saves/") + name + ".sav";
        return true;
    }

    bool validLightLevel(int value)
    {
        return value >= static_cast<int>(LightLevel::Bright) &&
            value <= static_cast<int>(LightLevel::Off);
    }
}

const char* SaveManager::getSavePath()
{
    return "saves/campaign.sav";
}

bool SaveManager::save(Campaign& campaign, std::string& message)
{
    return save(campaign, "campaign", message);
}

bool SaveManager::load(Campaign& campaign, std::string& message)
{
    return load(campaign, "campaign.sav", message);
}

std::vector<std::string> SaveManager::listSaves()
{
    std::vector<std::string> saves;
    std::error_code error;
    if (!std::filesystem::exists("saves", error) || error ||
        !std::filesystem::is_directory("saves", error) || error)
    {
        return saves;
    }

    for (std::filesystem::directory_iterator iterator("saves", error), end;
         !error && iterator != end; iterator.increment(error))
    {
        std::error_code itemError;
        if (!iterator->is_regular_file(itemError) || itemError ||
            iterator->path().extension() != ".sav")
        {
            continue;
        }

        const std::string filename = iterator->path().filename().string();
        std::string validatedPath;
        if (buildSavePath(filename, validatedPath))
        {
            saves.push_back(filename);
        }
    }

    std::sort(saves.begin(), saves.end());
    return saves;
}

bool SaveManager::save(Campaign& campaign, const std::string& saveName, std::string& message)
{
    std::string savePath;
    if (!buildSavePath(saveName, savePath))
    {
        message = "Invalid save name. Use only letters, numbers, underscores, or hyphens.";
        return false;
    }
    std::error_code directoryError;
    std::filesystem::create_directories("saves", directoryError);
    if (directoryError)
    {
        message = "Could not create the saves folder: " + directoryError.message();
        return false;
    }

    const std::string temporaryPath = savePath + ".tmp";
    std::ofstream output(temporaryPath.c_str(), std::ios::out | std::ios::trunc);
    if (!output)
    {
        message = "Could not open the save file for writing.";
        return false;
    }

    const Inventory& inventory = campaign.getInventory();
    output << "VOC_CAMPAIGN " << SAVE_VERSION << '\n';
    output << campaign.getGold() << '\n';
    output << inventory.getFood() << ' '
           << inventory.getTorches() << ' '
           << inventory.getMedicine() << '\n';
    output << static_cast<int>(campaign.getLightLevel()) << '\n';
    output << (campaign.hasUsedStageCoach() ? 1 : 0) << '\n';
    output << campaign.getRosterSize() << '\n';

    for (int i = 1; i <= campaign.getRosterSize(); ++i)
    {
        Character* hero = campaign.getRosterMember(i);
        const char* type = hero == nullptr ? nullptr : getHeroType(*hero);
        if (hero == nullptr || type == nullptr)
        {
            output.close();
            std::filesystem::remove(temporaryPath);
            message = "The roster contains an unsupported hero type; nothing was saved.";
            return false;
        }

        output << type << ' '
               << hero->getHP() << ' '
               << hero->getStress() << ' '
               << campaign.getParty().getPosition(*hero) << '\n';
    }

    output.flush();
    if (!output)
    {
        output.close();
        std::filesystem::remove(temporaryPath);
        message = "A write error occurred; the existing save was left untouched.";
        return false;
    }
    output.close();

    std::error_code renameError;
    std::filesystem::rename(temporaryPath, savePath, renameError);
    if (renameError)
    {
        // Some platforms do not replace an existing destination on rename.
        std::error_code removeError;
        std::filesystem::remove(savePath, removeError);
        renameError.clear();
        std::filesystem::rename(temporaryPath, savePath, renameError);
    }

    if (renameError)
    {
        std::filesystem::remove(temporaryPath);
        message = "Could not finalize the save file: " + renameError.message();
        return false;
    }

    message = "Campaign saved to " + savePath + ".";
    return true;
}

bool SaveManager::load(Campaign& campaign, const std::string& saveFileName, std::string& message)
{
    std::string savePath;
    if (!buildSavePath(saveFileName, savePath))
    {
        message = "Invalid save file selection.";
        return false;
    }

    std::ifstream input(savePath.c_str());
    if (!input)
    {
        message = "No save file was found at " + savePath + ".";
        return false;
    }

    std::string signature;
    int version = 0;
    int gold = 0;
    int food = 0;
    int torches = 0;
    int medicine = 0;
    int lightValue = 0;
    int stageCoachUsed = 0;
    int rosterCount = 0;

    if (!(input >> signature >> version) ||
        signature != "VOC_CAMPAIGN" || version != SAVE_VERSION ||
        !(input >> gold >> food >> torches >> medicine
                >> lightValue >> stageCoachUsed >> rosterCount))
    {
        message = "The save file is invalid or uses an unsupported version. The current campaign was not changed.";
        return false;
    }

    if (gold < 0 || food < 0 || torches < 0 || medicine < 0 ||
        !validLightLevel(lightValue) ||
        (stageCoachUsed != 0 && stageCoachUsed != 1) ||
        rosterCount < 0 || static_cast<std::size_t>(rosterCount) > MAX_ROSTER_SIZE)
    {
        message = "The save file contains invalid campaign values. The current campaign was not changed.";
        return false;
    }

    std::vector<SavedHero> savedHeroes;
    bool occupiedPositions[5] = { false, false, false, false, false };

    for (int i = 0; i < rosterCount; ++i)
    {
        std::string type;
        int hp = 0;
        int stress = 0;
        int position = 0;

        if (!(input >> type >> hp >> stress >> position))
        {
            message = "The save file ended before all heroes could be read. The current campaign was not changed.";
            return false;
        }

        std::shared_ptr<Character> hero = createHero(type);
        if (!hero || hp < 0 || hp > hero->getMaxHP() ||
            stress < 0 || stress > 200 || position < 0 || position > 4)
        {
            message = "The save file contains an invalid hero record. The current campaign was not changed.";
            return false;
        }

        if (position != 0 && occupiedPositions[position])
        {
            message = "The save file assigns multiple heroes to one formation position. The current campaign was not changed.";
            return false;
        }
        if (position != 0)
        {
            occupiedPositions[position] = true;
        }

        savedHeroes.push_back({ hero, hp, stress, position });
    }

    std::string extraData;
    if (input >> extraData)
    {
        message = "The save file contains unexpected trailing data. The current campaign was not changed.";
        return false;
    }

    // All input is validated before the live campaign is modified.
    while (campaign.getRosterSize() > 0)
    {
        campaign.dismissHero(campaign.getRosterMember(1));
    }

    for (std::size_t i = 0; i < savedHeroes.size(); ++i)
    {
        SavedHero& saved = savedHeroes[i];
        saved.hero->restoreSavedState(saved.hp, saved.stress);
        Character* loadedHero = campaign.addHero(saved.hero);
        if (saved.position != 0 && loadedHero != nullptr)
        {
            campaign.getParty().addMember(*loadedHero, saved.position);
        }
    }

    campaign.getInventory() = Inventory(food, torches, medicine);
    campaign.getGold() = gold;
    campaign.getLightLevel() = static_cast<LightLevel>(lightValue);
    if (stageCoachUsed == 1)
    {
        campaign.markStageCoachUsed();
    }
    else
    {
        campaign.resetStageCoachUsed();
    }

    message = "Campaign loaded from " + savePath + ".";
    return true;
}
