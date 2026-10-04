#pragma once
#include <iostream>
#include <string>
#include <vector>

class ConsoleRenderer {
private:
    int width;
    int height;
    std::vector<std::string> screenBuffer;

public:
    ConsoleRenderer(int w, int h) : width(w), height(h) {
        // Enable ANSI sequences for Windows 10/11 or Linux
        std::cout << "\x1b[?25l"; // Hide cursor
        clearBuffer();
    }

    ~ConsoleRenderer() {
        std::cout << "\x1b[?25h"; // Restore cursor
    }

    void clearBuffer() {
        screenBuffer.assign(height, std::string(width, ' '));
    }

    void drawString(int x, int y, const std::string& str) {
        if (y < 0 || y >= height) return;
        for (size_t i = 0; i < str.length(); ++i) {
            if (x + i >= 0 && x + i < (size_t)width) {
                screenBuffer[y][x + i] = str[i];
            }
        }
    }

    void render() {
        // Move cursor to 0,0 instead of system("cls")
        std::cout << "\x1b[H";
        std::string frame = "";
        for (const auto& row : screenBuffer) {
            frame += row + "\n";
        }
        std::cout << frame << std::flush;
    }
};


