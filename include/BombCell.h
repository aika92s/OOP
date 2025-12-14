#ifndef MINESWEEPERLAB_BOMBCELL_H
#define MINESWEEPERLAB_BOMBCELL_H

#include "Board.h"
#include "ICellContent.h"

//represents a cell with a default bomb
class BombCell : public ICellContent {
public:
    CellType getType() const override { return CellType::Bomb; }
    signed char getValue() const override { return -1; }
    void onReveal(Board &board, int x, int y) override;

    std::unique_ptr<ICellContent> incrementValue() override;
};

/* represents a cell with a bomb that doesn't finish the game
it randomly toggles flags on the board */
class ChaosBomb final : public BombCell {
public:
    void onReveal(Board &board, int x, int y) override;
    CellType getType() const override { return CellType::ChaosBomb; }

};

#endif //MINESWEEPERLAB_BOMBCELL_H