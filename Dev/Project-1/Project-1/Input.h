#pragma once

#include <string>

namespace Input
{
    int getInt(const std::string& prompt);

    int getIntInRange(
        const std::string& prompt,
        int minimum,
        int maximum
    );

    void waitForEnter();
}