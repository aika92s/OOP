#include "Cell.h"
#include "Board.h"

signed char Cell::getDisplayChar() const {
    if (is_revealed_) {
        const signed char value = content_->getValue();
        if (value == 0) return ' ';
        if (value == -1) return 'B';
        return '0' + value;
    }

    if (is_flagged_) return 'F';

    return 'X';
}

void Cell::setContent(std::unique_ptr<ICellContent> content) {
    content_ = std::move(content);
}

void Cell::reveal(Board &board, int x, int y) {
    if (is_revealed_) return;
    if (is_flagged_) return;

    if (!content_->isBomb()) {
        board.decrementSaveCell();
    }

    is_revealed_ = true;
    content_->onReveal(board, x, y);
}

void Cell::toggleFlag() {
    if (is_revealed_) return;
    is_flagged_ = !(is_flagged_);
}

void Cell::resetState() {
    is_flagged_ = false;
    is_revealed_ = false;
}
