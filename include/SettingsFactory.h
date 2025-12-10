#ifndef MINESWEEPERLAB_SETTINGSFACTORY_H
#define MINESWEEPERLAB_SETTINGSFACTORY_H

#include <memory>
#include "GameDifficulty.h"
#include "IGameSettings.h"
#include "ObjectFactory.h"
#include "SettingsFactoryPolicies.h"

class SettingsFactory {
public:
    static void menu();
    static std::unique_ptr<IGameSettings> create(GameDifficulty mode);

private:
    using SettingsFactoryType = Factory<
        IGameSettings,
        GameDifficulty,
        std::function<std::unique_ptr<IGameSettings>()>,
        DefaultToBeginnerPolicy
    >;

    static SettingsFactoryType& GetInstance();
};

#endif //MINESWEEPERLAB_SETTINGSFACTORY_H