#ifndef MINESWEEPERLAB_EMPTYCELL_H
#define MINESWEEPERLAB_EMPTYCELL_H

#include "Board.h"
#include "NumberedCell.h"
#include "ICellContent.h"

//represents a cell without a bomb or number
class EmptyCell final : public ICellContent {
public:
    bool isBomb() const override { return false; }
    signed char getValue() const override { return 0; }
    void onReveal(Board &board, int x, int y) override;
    std::unique_ptr<ICellContent> incrementValue() override;
};


#endif //MINESWEEPERLAB_EMPTYCELL_H