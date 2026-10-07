#include "Input.h"

#include <iostream>

namespace Input
{
    int getInt(const std::string& prompt)
    {
        while (true)
        {
            std::cout << prompt;

            std::string input;
            std::getline(std::cin, input);

            try
            {
                size_t position = 0;
                int value = std::stoi(input, &position);

                // Make sure there wasn't extra junk after the number.
                if (position == input.length())
                {
                    return value;
                }
            }
            catch (...)
            {
                // Conversion failed.
            }

            std::cout << "Invalid input. Please enter a number.\n";
        }
    }

    int getIntInRange(
        const std::string& prompt,
        int minimum,
        int maximum
    )
    {
        while (true)
        {
            int value = getInt(prompt);

            if (value >= minimum && value <= maximum)
            {
                return value;
            }

            std::cout
                << "Please enter a number between "
                << minimum
                << " and "
                << maximum
                << ".\n";
        }
    }

    void waitForEnter()
    {
        std::cout << "\nPress Enter to continue...";
        std::string input;
        std::getline(std::cin, input);
    }
}