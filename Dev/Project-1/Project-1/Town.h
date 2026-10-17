#pragma once

class Campaign;

// Main campaign hub. Each activity returns to this town menu when complete.
class Town
{
public:
    void run(Campaign& campaign);
};
