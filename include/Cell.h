#ifndef MINESWEEPERLAB_CELL_H
#define MINESWEEPERLAB_CELL_H
#include <memory>
#include "ICellContent.h"

/*the complete state of the cell:
combines the cell state (open, flag) and its contents
*/
class Cell {
    friend class Board;
    std::unique_ptr<ICellContent> content_;
    bool is_revealed_;
    bool is_flagged_;

    signed char getDisplayChar() const;
    void setContent(std::unique_ptr<ICellContent> content);
    bool isRevealed() const { return is_revealed_; }
    bool isFlagged() const { return is_flagged_; }

public:
    void reveal(Game& game, Board& board, int x, int y);
    void toggleFlag(); //changes is_flagged to opposite value
};


#endif

