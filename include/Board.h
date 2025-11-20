#ifndef MINESWEEPERLAB_BOARD_H
#define MINESWEEPERLAB_BOARD_H
#include <vector>
#include "Cell.h"
class Board {
    friend class EmptyCell;
    friend class Cell;
    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
    std::vector<std::pair<int, int>> bomb_locations_;
    int total_bombs_;
    int safe_cells_remaining_;

    int getWidth() const;
    int getHeight() const;
    void decrementSaveCell(Game &game);
    void revealAllBombs();
    void addValueToAdjacentCells(int x, int y);
    int countAdjacentFlags(int x, int y) const;
    void expandEmpty(Game& game, int x, int y);
    bool isSafeZone(int x, int y, int click_x, int click_y);
    void revealAdjacentCells(Game &game, int x, int y);
public:
    void populate(Game &game, int first_click_x, int first_click_y); //first click
    void revealCell(Game& game, int x, int y);
    void flagCell(int x, int y);
    void chordCell(Game& game, int x, int y);
    bool isValid(int x, int y) const;

    Board(int w, int h, int bombs);
};


#endif
