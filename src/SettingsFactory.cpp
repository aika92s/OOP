#include "SettingsFactory.h"
#include "GameSettings.h"
#include <iostream>

SettingsFactory::SettingsFactoryType& SettingsFactory::GetInstance() {
    static SettingsFactoryType factory;
    static bool registered = false;

    if (!registered) {
        factory.Register(GameDifficulty::BEGINNER, []() {
            return std::make_unique<BeginnerGameSettings>();
        });
        factory.Register(GameDifficulty::INTERMEDIATE, []() {
            return std::make_unique<IntermediateGameSettings>();
        });
        factory.Register(GameDifficulty::EXPERT, []() {
            return std::make_unique<ExpertGameSettings>();
        });
        registered = true;
    }
    return factory;
}

std::unique_ptr<IGameSettings> SettingsFactory::create(GameDifficulty mode) {
    return GetInstance().CreateObject(mode);
}

void SettingsFactory::menu() {
    std::cout << "=== MINESWEEPER ===\n";
    std::cout << "Select difficulty:\n";
    std::cout << "1 - Beginner\n";
    std::cout << "2 - Intermediate\n";
    std::cout << "3 - Expert\n";
    std::cout << "> ";
}