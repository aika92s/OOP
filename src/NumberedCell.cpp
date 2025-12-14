#include "NumberedCell.h"

std::unique_ptr<ICellContent> NumberedCell::incrementValue() {
    number_++;
    return nullptr;
}

NumberedCell::NumberedCell(const signed char number)  : number_(number) {}