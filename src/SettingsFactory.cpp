#include "SettingsFactory.h"
#include <iostream>
#include <unordered_map>
#include <functional>
#include "GameSettings.h"

std::unique_ptr<IGameSettings> SettingsFactory::create(GameDifficulty mode) {
    static const std::unordered_map<
                GameDifficulty,
                std::function<std::unique_ptr<IGameSettings>()>>

    settingsCreators = {
        { GameDifficulty::BEGINNER,     []() { return std::make_unique<BeginnerGameSettings>(); } },
        { GameDifficulty::INTERMEDIATE, []() { return std::make_unique<IntermediateGameSettings>(); } },
        { GameDifficulty::EXPERT,       []() { return std::make_unique<ExpertGameSettings>(); } }
    };

    auto it = settingsCreators.find(mode);
    if (it != settingsCreators.end()) {
        return it->second();
    }

    std::cout << "Unknown mode, defaulting to Beginner.\n";
    return std::make_unique<BeginnerGameSettings>();

}

void SettingsFactory::menu() {
    std::cout << "=== MINESWEEPER ===\n";
    std::cout << "Select difficulty:\n";
    std::cout << "1 - Beginner\n";
    std::cout << "2 - Intermediate\n";
    std::cout << "3 - Expert\n";
    std::cout << "> ";
}