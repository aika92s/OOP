#ifndef MINESWEEPERLAB_GAMEVIEW_H
#define MINESWEEPERLAB_GAMEVIEW_H
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "GameState.h"

class GameView {
    sf::RenderWindow* window_;
    const int WINDOW_MARGIN;
    const int CELL_SIZE;
    sf::Font font_;

    sf::Text text(bool bold, unsigned int size, const sf::Color &color, const std::string &str) const;

    void overlay(GameState current_state, const Board &board) const;

public:
    GameView(sf::RenderWindow* window, const int cell_size, const int window_margin);
    void initializeGraphics();
    void cellColor(sf::RectangleShape &rect, const Board::Cell &cell, int x, int y, int start_offset, GameState current_state) const;
    bool isNumber(char display_char) const;

    bool isBomb(char display_char) const;

    void cellText(sf::RectangleShape &rect, const Board::Cell &cell);
    void render(const Board &board);
    int getPixel(int cell) const;
    int getCell(int pixel) const;
};

#endif //MINESWEEPERLAB_GAMEVIEW_H