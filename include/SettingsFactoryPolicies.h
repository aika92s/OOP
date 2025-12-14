#ifndef MINESWEEPERLAB_SETTINGSFACTORYPOLICIES_H
#define MINESWEEPERLAB_SETTINGSFACTORYPOLICIES_H

#include "GameDifficulty.h"
#include "IGameSettings.h"
#include "GameSettings.h"
#include <iostream>

class DefaultToBeginnerPolicy {
public:
    static std::unique_ptr<IGameSettings> OnUnknownType(const GameDifficulty& id) {
        std::cout << "Unknown mode, defaulting to Beginner.\n";
        return std::make_unique<BeginnerGameSettings>();
    }
};

#endif //MINESWEPERLAB_SETTINGSFACTORYPOLICIES_H