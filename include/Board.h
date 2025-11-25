#ifndef MINESWEEPERLAB_BOARD_H
#define MINESWEEPERLAB_BOARD_H
#include <vector>
#include <random>
#include "Cell.h"
#include "GameState.h"

class Board {
    friend class EmptyCell;
    friend class Cell;
    friend class GameView;
    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
    std::vector<std::pair<int, int>> bomb_locations_;
    int total_bombs_;
    int safe_cells_remaining_;
    GameState current_state_ = GameState::READY;
    std::mt19937 generator_;

    void decrementSaveCell();
    void addValueToAdjacentCells(int x, int y);
    int countAdjacentFlags(int x, int y) const;
    void expandEmpty(int x, int y);
    bool isSafeZone(int x, int y, int click_x, int click_y);
    void revealAdjacentCells(int x, int y);
public:
    void populate(int first_click_x, int first_click_y); //first click
    void revealCell(int x, int y);
    void flagCell(int x, int y);
    void chordCell(int x, int y);
    bool isValid(int x, int y) const;

    GameState getGameState() const;

    void setGameState(GameState state);

    const Cell &getCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    void reset();
    void revealAllBombs();

    void randomlyToggleFlags();

    int getRandomValue();

    Board(int w, int h, int bombs);
};


#endif