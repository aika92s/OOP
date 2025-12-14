#ifndef MINESWEEPERLAB_GAMESETTINGS_H
#define MINESWEEPERLAB_GAMESETTINGS_H
#include "IGameSettings.h"

class BeginnerGameSettings final : public IGameSettings {
    int getWidth() const override { return 10; }
    int getHeight() const override { return 10; }
    int getBombsCount() const override { return 15; }
};

class IntermediateGameSettings final : public IGameSettings {
    int getWidth() const override { return 16; }
    int getHeight() const override { return 16; }
    int getBombsCount() const override { return 40; }
};

class ExpertGameSettings final : public IGameSettings {
    int getWidth() const override { return 16; }
    int getHeight() const override { return 30; }
    int getBombsCount() const override { return 99; }
};


#endif //MINESWEEPERLAB_GAMESETTINGS_H