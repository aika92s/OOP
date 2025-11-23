#include "Game.h"
#include <iostream>

int main() {

    const int width = 10;
    const int height = 10;
    const int bombs = 15;

    if (bombs >= width * height) {
        std::cerr << "Error: Too many bombs for the selected board size.\n";
        return 1;
    }

    try {
        Game minesweeper(width, height, bombs);
        minesweeper.run();

    } catch (const std::exception& e) {
        std::cerr << "Critical Error during game execution: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}