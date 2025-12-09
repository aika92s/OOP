#ifndef MINESWEEPERLAB_CELLCONTENT_H
#define MINESWEEPERLAB_CELLCONTENT_H
#include "Board.h"
#include "ICellContent.h"

//represents a cell with a number
class NumberedCell final: public ICellContent {
    signed char number_;
public:
    bool isBomb() const override { return false; }
    signed char getValue() const override {return number_; }
    void onReveal(Board &board, int x, int y) override {}
    std::unique_ptr<ICellContent> incrementValue() override;
    ~NumberedCell() override = default;
    explicit NumberedCell(const signed char number);
};
#endif