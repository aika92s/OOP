#ifndef MINESWEEPERLAB_GAME_H
#define MINESWEEPERLAB_GAME_H
#include "Board.h"
#include <SFML/Graphics.hpp>
#include "GameView.h"
#include "IGameSettings.h"

class Game {
    Board game_board_;
    sf::RenderWindow window_;
    GameView view_;
    static const int WINDOW_MARGIN = 50;
    static const int CELL_SIZE = 32;

    void resetGame();
    /*pixels to cords of grid
     converts a mouse button to a mode*/
    void processInput();
    /*checks the boundaries, game state.
     calls the board member function (populate, revealCell, flagCell).*/
    void processClick(int cell_x, int cell_y, bool is_flag_mode);
public:
    void run();
    Game(const IGameSettings& settings);

};


#endif
