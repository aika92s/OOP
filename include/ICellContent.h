#ifndef OOP_ICELLCONTENT_H
#define OOP_ICELLCONTENT_H

/*information about the cell (content like a bomb, a number or emptiness):

isBomb() - is there a bomb in the cell?

getValue() - returns a number (0-8incrementValue() or -1 (if the cell contains a bomb)) that
indicates the number of bombs in adjacent cells.

onReveal(...) - reveals the cell.

incrementValue() - if cell is empty it returns ptr to new object (cell with number with value=1)
if cell contains bomb nothing happens
if cell contains number it increase the value (value++)
*/
class Board;

class ICellContent {
public:
    virtual ~ICellContent() = default;

    virtual bool isBomb() const = 0;
    virtual signed char getValue() const = 0;
    virtual void onReveal(Board &board, int x, int y) = 0;
    virtual std::unique_ptr<ICellContent> incrementValue() = 0;
};

#endif
