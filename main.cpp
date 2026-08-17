#include "chess.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <array>
#include <cctype>

namespace {

void printBanner() {
    std::cout <<
        "==================================\n"
        "        C++ CHESS GAME\n"
        "==================================\n\n";
}

void printDivider() {
    std::cout << "----------------------------------\n";
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    // trim whitespace
    line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
    line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), line.end());
    return line;
}

int chooseGameMode() {
    printDivider();
    std::cout << "  Select Game Mode\n";
    printDivider();
    std::cout << "  [1] Player vs Player\n";
    std::cout << "  [2] Player vs CPU (Stockfish)\n";
    printDivider();

    while (true) {
        std::string choice = readLine("  Enter choice (1-2): ");
        if (choice == "1") return 1;
        if (choice == "2") return 2;
        std::cout << "  Invalid choice. Please enter 1 or 2.\n";
    }
}

Color chooseCpuColor() {
    printDivider();
    std::cout << "  Choose CPU Side\n";
    printDivider();
    std::cout << "  [w] CPU plays White\n";
    std::cout << "  [b] CPU plays Black\n";
    printDivider();

    while (true) {
        std::string choice = readLine("  Enter choice (w/b): ");
        if (!choice.empty()) {
            char c = std::tolower(static_cast<unsigned char>(choice[0]));
            if (c == 'w') return Color::WHITE;
            if (c == 'b') return Color::BLACK;
        }
        std::cout << "  Invalid choice. Please enter 'w' or 'b'.\n";
    }
}

struct Difficulty {
    std::string label;
    int skillLevel;
    int searchDepth;
};

// Friendly difficulty presets mapped to Stockfish "Skill Level" (0-20) and search depth.
const std::array<Difficulty, 5> kDifficulties{{
    {"Beginner",     3,  5},
    {"Easy",         6,  8},
    {"Intermediate", 10, 10},
    {"Hard",         15, 13},
    {"Expert",       20, 18},
}};

Difficulty chooseDifficulty() {
    printDivider();
    std::cout << "  Select CPU Difficulty\n";
    printDivider();
    for (size_t i = 0; i < kDifficulties.size(); i++) {
        std::cout << "  [" << (i + 1) << "] " << kDifficulties[i].label << "\n";
    }
    printDivider();

    while (true) {
        std::string choice = readLine("  Enter choice (1-" + std::to_string(kDifficulties.size()) + "): ");
        try {
            int idx = std::stoi(choice);
            if (idx >= 1 && idx <= static_cast<int>(kDifficulties.size())) {
                return kDifficulties[idx - 1];
            }
        } catch (...) {
            // fall through to error message below
        }
        std::cout << "  Invalid choice. Please enter a number from the list.\n";
    }
}

} // namespace

int main() {
    try {
        printBanner();
        int mode = chooseGameMode();

        if (mode == 2) {
            Color cpuColor = chooseCpuColor();
            Difficulty difficulty = chooseDifficulty();

            std::cout << "\n";
            printDivider();
            std::cout << "  CPU: " << difficulty.label
                       << " (" << (cpuColor == Color::WHITE ? "White" : "Black") << ")\n";
            printDivider();
            std::cout << "\n";

            ChessGame game(true, cpuColor, difficulty.skillLevel, difficulty.searchDepth);
            game.startGame();
        } else {
            std::cout << "\n";
            ChessGame game(false);  // Player vs Player
            game.startGame();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}