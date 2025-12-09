#include "EmptyCell.h"

void EmptyCell::onReveal(Board &board, int x, int y) {
    board.expandEmpty(x, y);
}

std::unique_ptr<ICellContent> EmptyCell::incrementValue() {
    return std::make_unique<NumberedCell>(1);
}

