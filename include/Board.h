#ifndef MINESWEEPERLAB_BOARD_H
#define MINESWEEPERLAB_BOARD_H
#include <memory>
#include <vector>
#include <random>
#include "GameState.h"
#include "Board.h"
#include "ICellContent.h"

class Board {
public:
    /*the complete state of the cell:
combines the cell state (open, flag) and its contents */
    class Cell {
        std::unique_ptr<ICellContent> content_;
        bool is_revealed_;
        bool is_flagged_;

    public:
        std::unique_ptr<ICellContent> incrementValue();
        void setReveal();
        void setContent(std::unique_ptr<ICellContent> content);
        bool isBomb(signed char value) const;
        signed char getDisplayChar() const;
        void reveal(Board& board, int x, int y);
        void toggleFlag(); //changes is_flagged to opposite value
        bool isRevealed() const;
        bool isFlagged() const;
        void resetState();
        bool isBomb() const;

        signed char getValue() const;
    };

private:

    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
    std::vector<std::pair<int, int>> bomb_locations_;
    int total_bombs_;
    int safe_cells_remaining_;
    GameState current_state_ = GameState::READY;
    std::mt19937 generator_;

    void addValueToAdjacentCells(int x, int y);
    int countAdjacentFlags(int x, int y) const;
    bool isSafeZone(int x, int y, int click_x, int click_y);
    void revealAdjacentCells(int x, int y);

public:

    void decrementSaveCell();
    void populate(int first_click_x, int first_click_y); //first click
    void revealCell(int x, int y);
    void flagCell(int x, int y);
    void chordCell(int x, int y);
    bool isValid(int x, int y) const;
    void expandEmpty(int x, int y);
    GameState getGameState() const;
    void setGameState(GameState state);
    const Cell& getCell(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    void reset();
    void revealAllBombs();
    void randomlyToggleFlags();
    int getRandomValue();

    Board(int w, int h, int bombs);

};


#endif