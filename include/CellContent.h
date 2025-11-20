#ifndef MINESWEEPERLAB_CELLCONTENT_H
#define MINESWEEPERLAB_CELLCONTENT_H
#include "Board.h"
#include "Game.h"
#include "ICellContent.h"

//represents a cell with a number
class NumberedCell final: public ICellContent {
    signed char number_;
public:
    bool isBomb() const override { return false; }
    signed char getValue() const override {return number_; }
    void onReveal(Game &game, Board &board, int x, int y) override;
    std::unique_ptr<ICellContent> incrementValue() override {
        number_++;
        return nullptr;
    }

    explicit NumberedCell(const signed char number) : number_(number) {}
};

//represents a cell without a bomb or number
class EmptyCell final : public ICellContent {
public:
    bool isBomb() const override { return false; }
    signed char getValue() const override { return 0; }
    void onReveal(Game &game, Board &board, int x, int y) override { board.expandEmpty(game, x, y); }
    std::unique_ptr<ICellContent> incrementValue() override {return std::make_unique<NumberedCell>(1); }
};

//represents a cell with a bomb
class BombCell final: public ICellContent {
 public:
    bool isBomb() const override { return true; }
    signed char getValue() const override { return -1; }
    void onReveal(Game &game, Board &board, int x, int y) override {
        game.setGameState(GameState::LOST);
        board.revealAllBombs();
    }

    std::unique_ptr<ICellContent> incrementValue() override {
        return nullptr;
    }
};

#endif
