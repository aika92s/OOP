#include "BombCell.h"

void BombCell::onReveal(Board &board, int x, int y) {
    board.setGameState(GameState::LOST);
    board.revealAllBombs();
}

std::unique_ptr<ICellContent> BombCell::incrementValue() {
    return nullptr;
}

void ChaosBomb::onReveal(Board &board, int x, int y) {
    board.randomlyToggleFlags();
}