#ifndef MINESWEEPERLAB_GAME_H
#define MINESWEEPERLAB_GAME_H
#include "Board.h"
#include "GameState.h"
#include <SFML/Graphics.hpp>

class Game {
    Board game_board_;
    GameState current_state_;
    const int CELL_SIZE = 32;
    sf::Font font_;

    void resetGame();
    void initializeGraphics(); //one-time setup of graphic resources required for rendering
    void displayBoard(sf::RenderWindow& window); //draws the current state of the playing field
    /*pixels to cords of grid
     converts a mouse button to a mode*/
    void processMouseClick(sf::Mouse::Button, int cell_x, int cell_y);
    /*checks the boundaries, game state.
     calls the board member function (populate, revealCell, flagCell).*/
    void processClick(int cell_x, int cell_y, bool is_flag_mode);

    void cellColor(sf::RenderWindow &window, sf::RectangleShape &rect, const Cell &cell, int x, int y, int start_offset);

    void cellText(sf::RenderWindow &window, sf::RectangleShape &rect, const Cell &cell);

public:
    void run();
    void setGameState(GameState state);
    Game(int w, int h, int bombs);

};


#endif
