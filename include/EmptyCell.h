#ifndef MINESWEEPERLAB_EMPTYCELL_H
#define MINESWEEPERLAB_EMPTYCELL_H

#include "Board.h"
#include "ICellContent.h"

//represents a cell without a bomb or number
class EmptyCell final : public ICellContent {
public:
    signed char getValue() const override { return 0; }
    CellType getType() const override { return CellType::Empty; }
    void onReveal(Board &board, int x, int y) override;
    std::unique_ptr<ICellContent> incrementValue() override;
};


#endif //MINESWEEPERLAB_EMPTYCELL_H