#include <algorithm>
#include <stdexcept>
#include "Board.h"
#include "CellContent.h"
#include "GameState.h"

const Cell &Board::getCell(int x, int y) const {
    return grid_[y][x];
}

int Board::getWidth() const {
    return width_;
}

int Board::getHeight() const {
    return height_;
}

void Board::reset() {
    for (int column = 0; column < height_; column++) {

        for (int row = 0; row < width_; row++) {
            grid_[column][row].resetState();
            grid_[column][row].setContent(std::make_unique<EmptyCell>());
        }
    }
    safe_cells_remaining_ = height_ * width_ - total_bombs_;
    bomb_locations_.clear();
    current_state_ = GameState::READY;
}

void Board::decrementSaveCell() {
    safe_cells_remaining_--;

    if (safe_cells_remaining_ == 0) current_state_ = GameState::WON;
}

void Board::revealAllBombs() {
    for (auto pair : bomb_locations_) {
        grid_[pair.second][pair.first].is_revealed_ = true;
    }
}

void Board::randomlyToggleFlags() {
    int numberOfToggleFlags = safe_cells_remaining_ / 4;
    std::vector<std::pair<int, int>> potential_spots;
    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (!grid_[y][x].isRevealed())
                potential_spots.push_back(std::make_pair(x, y));
        }
    }
    std::shuffle(potential_spots.begin(), potential_spots.end(), generator_);

    for (int i = 0; i < numberOfToggleFlags; i++) {
        int x = potential_spots[i].first;
        int y = potential_spots[i].second;
        grid_[y][x].toggleFlag();
    }
}

int Board::getRandomValue() {
    std::uniform_int_distribution<int> distribution(4, 8);
    int random_value = distribution(generator_);

    return random_value;
}

void Board::addValueToAdjacentCells(int const x, int const y) {
    for (int x_cord = x - 1; x_cord <= x + 1; x_cord++) {
        for (int y_cord = y - 1; y_cord <= y + 1; y_cord++) {

            if (y_cord == y && x_cord == x) continue;
            if (!isValid(x_cord, y_cord)) continue;
            if (grid_[y_cord][x_cord].content_->isBomb()) continue;

            std::unique_ptr<ICellContent> new_content = grid_[y_cord][x_cord].content_->incrementValue();
            if (new_content == nullptr) continue;
            grid_[y_cord][x_cord].setContent(std::move(new_content));
        }
    }
}

int Board::countAdjacentFlags(int const x, int const y) const {
    int count = 0;
    for (int x_cord = x - 1; x_cord <= x + 1; x_cord++) {
        for (int y_cord = y - 1; y_cord <= y + 1; y_cord++) {
            if (y_cord == y && x_cord == x) continue;
            if (isValid(x_cord, y_cord)) count += static_cast<int>(grid_[y_cord][x_cord].isFlagged());
        }
    }
    return count;
}

void Board::expandEmpty(int const x, int const y) {
    revealAdjacentCells(x, y);
}

bool Board::isSafeZone(int const x, int const y, int const click_x, int const click_y) {
    return (std::abs(x - click_x) <= 1 && std::abs(y - click_y) <= 1);
}

void Board::revealAdjacentCells(int const x, int const y) {
    for (int x_cord = x - 1; x_cord <= x + 1; x_cord++) {
        for (int y_cord = y - 1; y_cord <= y + 1; y_cord++) {

            if (y_cord == y && x_cord == x) continue;
            if (!isValid(x_cord, y_cord)) continue;
            if (grid_[y_cord][x_cord].isFlagged() || grid_[y_cord][x_cord].isRevealed()) continue;

            grid_[y_cord][x_cord].reveal(*this, x_cord, y_cord);
        }
    }
}

void Board::populate(int const first_click_x, int const first_click_y) {
    std::vector<std::pair<int, int>> potential_spots;

    for (int x = 0; x < width_; x++) {
        for (int y = 0; y < height_; y++) {
            if (!isSafeZone(x, y, first_click_x, first_click_y))
                potential_spots.push_back(std::make_pair(x, y));
        }
    }

    if (static_cast<int>(potential_spots.size()) < total_bombs_) {
        throw std::invalid_argument("Cannot populate board: total_bombs is greater than the available safe area.");
    }

    int random_value = getRandomValue();

    std::shuffle(potential_spots.begin(), potential_spots.end(), generator_);

    for (int i = 0; i < total_bombs_; ++i) {
        int x = potential_spots[i].first;
        int y = potential_spots[i].second;

        if (i % random_value == 0) {
            grid_[y][x].setContent(std::make_unique<ChaosBomb>());
        } else { grid_[y][x].setContent(std::make_unique<BombCell>()); }

        bomb_locations_.push_back(potential_spots[i]);
        addValueToAdjacentCells(x, y);
    }
    setGameState(GameState::PLAYING);
    revealCell(first_click_x, first_click_y);
}

void Board::revealCell(int const x, int const y) {
    if (!isValid(x, y)) return;
    if (grid_[y][x].isFlagged()) return;
    if (grid_[y][x].isRevealed()) return;
    grid_[y][x].reveal(*this, x, y);
}

void Board::flagCell(int const x, int const y) {
    if (!isValid(x, y)) return;
    if (grid_[y][x].isRevealed()) return;
    grid_[y][x].toggleFlag();
}

void Board::chordCell(int const x, int const y) {
    if (!grid_[y][x].isRevealed()) return;

    signed char value = grid_[y][x].content_->getValue();
    if (!(value > 0 && value < 9)) return;
    if (countAdjacentFlags(x, y) != value) return;

    revealAdjacentCells(x, y);
}

bool Board::isValid(int const x, int const y) const {
    if (x < 0 || x >= width_) return false;
    if (y < 0 || y >= height_) return false;

    return true;
}

GameState Board::getGameState() const {
    return current_state_;
}
void Board::setGameState(GameState state) {
    current_state_ = state;
}

Board::Board(int const w, int const h, int const bombs) : width_(w), height_(h),
                                                          total_bombs_(bombs),
                                                          safe_cells_remaining_(w * h - bombs), generator_(std::random_device{}()) {
    grid_.resize(h);
    for (int column = 0; column < h; column++) {
        grid_[column].resize(w);
        for (int row = 0; row < w; row++) grid_[column][row].setContent(std::make_unique<EmptyCell>());
    }
}
