#include "SettingsFactory.h"
#include <iostream>
#include "GameSettings.h"

std::unique_ptr<IGameSettings> SettingsFactory::create(GameDifficulty mode) {
    switch (mode) {
        case GameDifficulty::BEGINNER:
            return std::make_unique<BeginnerGameSettings>();
        case GameDifficulty::INTERMEDIATE:
            return std::make_unique<IntermediateGameSettings>();
        case GameDifficulty::EXPERT:
            return std::make_unique<ExpertGameSettings>();
        default:
            std::cout << "Unknown mode, defaulting to Beginner.\n";
            return std::make_unique<BeginnerGameSettings>();
    }
}

void SettingsFactory::menu() {
    std::cout << "=== MINESWEEPER ===\n";
    std::cout << "Select difficulty:\n";
    std::cout << "1 - Beginner\n";
    std::cout << "2 - Intermediate\n";
    std::cout << "3 - Expert\n";
    std::cout << "> ";
}