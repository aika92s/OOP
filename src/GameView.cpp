#include "GameView.h"
#include <iostream>
#include "Colors.h"

void GameView::overlay(GameState current_state, const Board& board) const {
    if (!(current_state == GameState::WON) && !(current_state == GameState::LOST)) return;

    const int window_width = board.getWidth() * CELL_SIZE + WINDOW_MARGIN;
    const int window_height = board.getHeight() * CELL_SIZE + WINDOW_MARGIN;

    sf::RectangleShape overlay(sf::Vector2f(window_width, window_height));
    overlay.setFillColor(Colors::OverlayColor);
    window_->draw(overlay);

    sf::Text titleText;
    titleText.setFont(font_);
    titleText.setCharacterSize(50);
    titleText.setStyle(sf::Text::Bold);

    if (current_state == GameState::WON) {
        titleText.setFillColor(sf::Color::Green);
        titleText.setString("YOU WON!");
    } else {
        titleText.setFillColor(sf::Color::Red);
        titleText.setString("GAME OVER!");
    }

    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    titleText.setPosition(window_width / 2.0f, window_height / 2.0f - 30);

    sf::Text subText;
    subText.setFont(font_);
    subText.setCharacterSize(30);
    subText.setFillColor(sf::Color::Black);
    subText.setString("Press 'R' to restart");

    sf::FloatRect subRect = subText.getLocalBounds();
    subText.setOrigin(subRect.left + subRect.width / 2.0f, subRect.top + subRect.height / 2.0f);
    subText.setPosition(window_width / 2.0f, window_height / 2.0f + 30);

    window_->draw(titleText);
    window_->draw(subText);
}

GameView::GameView(sf::RenderWindow* window, const int cell_size, const int window_margin) : window_(window),
    WINDOW_MARGIN(window_margin),
    CELL_SIZE(cell_size) {

    initializeGraphics();
}

void GameView::initializeGraphics() {
    if (!font_.loadFromFile("../gui/Nunito-Black.ttf")) {
        std::cerr << "Error: Failed to load font!\n";
        exit(1);
    }
}


void GameView::cellColor(sf::RectangleShape &rect, const Cell& cell, const int x, const int y, const int start_offset, GameState current_state) const {
    bool is_finished = (current_state == GameState::WON || current_state == GameState::LOST);
    bool show_content = cell.isRevealed() || is_finished;
    char display_char = cell.getDisplayChar();

    rect.setPosition(x * CELL_SIZE + start_offset, y * CELL_SIZE + start_offset);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(Colors::OutlineColor);

    if (show_content) {
        if (display_char == 'B') {
            rect.setFillColor(sf::Color::Red);
            window_->draw(rect);
            return;
        }

        rect.setFillColor(Colors::RevealedCellColor);
        window_->draw(rect);
        return;
    }

    rect.setFillColor(Colors::HiddenCellColor);
    window_->draw(rect);
}

bool GameView::isNumber(char display_char) const {
    if (display_char >= '1' && display_char <= '8') return true;

    return false;
}

void GameView::cellText(sf::RectangleShape &rect, const Cell& cell) {
    char display_char = cell.getDisplayChar();

    sf::Text text;
    text.setFont(font_);
    text.setString(display_char);
    text.setCharacterSize(CELL_SIZE / 2);

    if (isNumber(display_char)) {
        text.setFillColor(sf::Color::Blue);

    } else text.setFillColor(sf::Color::Black);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(rect.getPosition().x + CELL_SIZE / 2.0f,
                     rect.getPosition().y + CELL_SIZE / 2.0f);

    window_->draw(text);
}


void GameView::render(const Board& board) {
    const int start_offset = WINDOW_MARGIN / 2;
    const GameState current_state = board.getGameState();

    for (int y = 0; y < board.getHeight(); ++y) {
        for (int x = 0; x < board.getWidth(); ++x) {
            sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            const Cell& cell = board.getCell(x, y);

            bool is_finished = (current_state == GameState::WON || current_state == GameState::LOST);
            bool show_content = cell.isRevealed() || is_finished;

            cellColor(rect, cell, x, y, start_offset, current_state);
            if (!show_content && !cell.isFlagged()) {
                continue;
            }
            cellText(rect, cell);
        }
    }
    overlay(current_state, board);
}

int GameView::getPixel(int cell) const {
    int pixel = cell * CELL_SIZE + (WINDOW_MARGIN / 2);
    return pixel;
}

int GameView::getCell(int pixel) const {
    int cell = (pixel - (WINDOW_MARGIN / 2)) / CELL_SIZE;
    return cell;
}
