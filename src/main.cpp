#include "Game.h"
#include <iostream>
#include "GameDifficulty.h"
#include "GameSettings.h"
#include "SettingsFactory.h"

int main() {
    SettingsFactory::menu();
    int input;
    if (!(std::cin >> input)) {
        input = 1;
    }
    auto mode = static_cast<GameDifficulty>(input);

    try {
        std::unique_ptr<IGameSettings> settings = SettingsFactory::create(mode);

        Game minesweeper(*settings);
        minesweeper.run();

    } catch (const std::exception& e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
