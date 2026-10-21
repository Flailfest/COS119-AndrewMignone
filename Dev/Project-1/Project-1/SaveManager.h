#pragma once

#include <string>
#include <vector>

class Campaign;

// Saves and restores persistent campaign state using a versioned text file.
class SaveManager
{
public:
    // Legacy/default single-slot helpers retained for existing callers.
    static const char* getSavePath();
    static bool save(Campaign& campaign, std::string& message);
    static bool load(Campaign& campaign, std::string& message);

    // Named save slots. Save names may contain letters, numbers, underscores,
    // and hyphens. A .sav extension is optional when saving.
    static std::vector<std::string> listSaves();
    static bool save(Campaign& campaign, const std::string& saveName, std::string& message);
    // load accepts a filename returned by listSaves(), such as "campaign.sav".
    static bool load(Campaign& campaign, const std::string& saveFileName, std::string& message);
};
