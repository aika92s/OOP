#ifndef MINESWEEPERLAB_CELL_H
#define MINESWEEPERLAB_CELL_H
#include <memory>
#include "ICellContent.h"

/*the complete state of the cell:
combines the cell state (open, flag) and its contents
*/
class Cell {
    friend class Board;
    friend class Game;
    std::unique_ptr<ICellContent> content_;
    bool is_revealed_;
    bool is_flagged_;

    void setContent(std::unique_ptr<ICellContent> content);

public:
    signed char getDisplayChar() const;
    void reveal(Board& board, int x, int y);
    void toggleFlag(); //changes is_flagged to opposite value
    bool isRevealed() const { return is_revealed_; }
    bool isFlagged() const { return is_flagged_; }
    void resetState();
};


#endif


