//
// Created by aika92 on 11/25/25.
//
#ifndef MINESWEEPERLAB_SETTINGSFACTORY_H
#define MINESWEEPERLAB_SETTINGSFACTORY_H
#include <memory>
#include "GameDifficulty.h"
#include "IGameSettings.h"

class SettingsFactory {
public:
    static void menu();
    static std::unique_ptr<IGameSettings> create(GameDifficulty mode);
};


#endif //MINESWEEPERLAB_SETTINGSFACTORY_H