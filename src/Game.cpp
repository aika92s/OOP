#include "Game.h"
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <SFML/Graphics.hpp>


const int WINDOW_MARGIN = 50;

Game::Game(int w, int h, int bombs) :
    game_board_(w, h, bombs)
{
    current_state_ = GameState::READY;

    initializeGraphics();
}

void Game::resetGame() {
    game_board_.reset();
    current_state_ = GameState::READY;
}

void Game::initializeGraphics() {
    if (!font_.loadFromFile("../gui/Nunito-Black.ttf")) {
        std::cerr << "Error: Failed to load font!\n";
        exit(1);
    }
}

void Game::setGameState(GameState state) {
    current_state_ = state;
}

void Game::run() {
    const int window_width = game_board_.getWidth() * CELL_SIZE + WINDOW_MARGIN;
    const int window_height = game_board_.getHeight() * CELL_SIZE + WINDOW_MARGIN;

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Minesweeper SFML", sf::Style::Close);
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (current_state_ == GameState::READY || current_state_ == GameState::PLAYING) {
                if (event.type == sf::Event::MouseButtonReleased) {
                    processMouseClick(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
                }
            } else {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    resetGame();
                }
            }
        }

        window.clear(sf::Color(240, 240, 240));
        displayBoard(window);

        if (current_state_ == GameState::WON || current_state_ == GameState::LOST) {
            sf::RectangleShape overlay(sf::Vector2f(window_width, window_height));
            overlay.setFillColor(sf::Color(255, 255, 255, 150));
            window.draw(overlay);

            sf::Text titleText;
            titleText.setFont(font_);
            titleText.setCharacterSize(50);
            titleText.setStyle(sf::Text::Bold);

            if (current_state_ == GameState::WON) {
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

            window.draw(titleText);
            window.draw(subText);
        }

        window.display();
    }
}

void Game::processMouseClick(sf::Mouse::Button button, int pixel_x, int pixel_y) {

    int cell_x = (pixel_x - WINDOW_MARGIN / 2) / CELL_SIZE;
    int cell_y = (pixel_y - WINDOW_MARGIN / 2) / CELL_SIZE;

    bool is_flag_mode = (button == sf::Mouse::Right);

    processClick(cell_x, cell_y, is_flag_mode);
}

void Game::processClick(int cell_x, int cell_y, bool is_flag_mode) {

    if (!game_board_.isValid(cell_x, cell_y)) {
        return;
    }

    if (current_state_ == GameState::READY) {
        if (is_flag_mode) {
            return;
        }

        try {
            game_board_.populate(*this, cell_x, cell_y);

            setGameState(GameState::PLAYING);
        } catch (const std::exception& e) {
            std::cerr << "Game Setup Error: " << e.what() << "\n";
            setGameState(GameState::LOST);
            return;
        }
    }

    if (current_state_ == GameState::PLAYING) {
        if (is_flag_mode) {
            game_board_.flagCell(cell_x, cell_y);
        } else {
            const Cell& cell = game_board_.getCell(cell_x, cell_y);

            if (cell.isRevealed() && cell.content_->getValue() > 0) {
                game_board_.chordCell(*this, cell_x, cell_y);
            } else {
                game_board_.revealCell(*this, cell_x, cell_y);
            }
        }
    }
}

void Game::cellColor(sf::RenderWindow& window, sf::RectangleShape &rect, const Cell& cell, const int x, const int y, const int start_offset) {
    bool is_finished = (current_state_ == GameState::WON || current_state_ == GameState::LOST);
    bool show_content = cell.isRevealed() || is_finished;
    char display_char = cell.getDisplayChar();

    rect.setPosition(x * CELL_SIZE + start_offset, y * CELL_SIZE + start_offset);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color(100, 100, 100));

    if (show_content) {
        if (display_char == 'B') {
            rect.setFillColor(sf::Color::Red);
            window.draw(rect);
            return;
        }

        rect.setFillColor(sf::Color(220, 220, 220));
    } else {
        rect.setFillColor(sf::Color(150, 150, 150));
    }

    window.draw(rect);
}

void Game::cellText(sf::RenderWindow& window, sf::RectangleShape &rect, const Cell& cell) {
    char display_char = cell.getDisplayChar();

    sf::Text text;
    text.setFont(font_);
    text.setString(display_char);
    text.setCharacterSize(CELL_SIZE / 2);

    if (display_char >= '1' && display_char <= '8') {
        text.setFillColor(sf::Color::Blue);

    } else text.setFillColor(sf::Color::Black);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(rect.getPosition().x + CELL_SIZE / 2.0f,
                           rect.getPosition().y + CELL_SIZE / 2.0f);

    window.draw(text);
}

void Game::displayBoard(sf::RenderWindow& window) {
    const int start_offset = WINDOW_MARGIN / 2;

    for (int y = 0; y < game_board_.getHeight(); ++y) {
        for (int x = 0; x < game_board_.getWidth(); ++x) {
            sf::RectangleShape rect(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            const Cell& cell = game_board_.getCell(x, y);

            bool is_finished = (current_state_ == GameState::WON || current_state_ == GameState::LOST);
            bool show_content = cell.isRevealed() || is_finished;

            cellColor(window, rect, cell, x, y, start_offset);
            if (!show_content && !cell.isFlagged()) {
                continue;
            }

            cellText(window, rect, cell);

        }
    }
}