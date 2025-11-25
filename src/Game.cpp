#include "Game.h"
#include <iostream>
#include "GameView.h"
#include <SFML/Graphics.hpp>

void Game::resetGame() {
    game_board_.reset();
}

void Game::run() {
    window_.setFramerateLimit(60);

    while (window_.isOpen()) {
        processInput();
        window_.clear(sf::Color(240, 240, 240));

        view_.render(game_board_);
        window_.display();
    }
}

Game::Game(const IGameSettings& settings) :
    game_board_(settings.getWidth(), settings.getHeight(), settings.getBombsCount()),
    window_(sf::VideoMode(CELL_SIZE * settings.getWidth()+ WINDOW_MARGIN, settings.getHeight() * CELL_SIZE + WINDOW_MARGIN),
    "Minesweeper", sf::Style::Close), view_(&window_, CELL_SIZE, WINDOW_MARGIN) {}

void Game::processInput() {
    sf::Event event;

    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
            return;
        }
        GameState current_state = game_board_.getGameState();
        bool playingCondition = current_state == GameState::READY || current_state == GameState::PLAYING;

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R && !playingCondition) {
            resetGame();
            continue;
        }

        if (event.type != sf::Event::MouseButtonReleased) continue;

        int pixel_x = event.mouseButton.x;
        int pixel_y = event.mouseButton.y;

        int cell_x = view_.getCell(pixel_x);
        int cell_y = view_.getCell(pixel_y);

        bool is_flag_mode = (event.mouseButton.button == sf::Mouse::Right);

        processClick(cell_x, cell_y, is_flag_mode);
    }
}

void Game::processClick(int cell_x, int cell_y, bool is_flag_mode) {
    GameState currentState = game_board_.getGameState();
    if (currentState == GameState::WON || currentState == GameState::LOST) return;

    if (!game_board_.isValid(cell_x, cell_y)) return;

    if (currentState == GameState::READY && is_flag_mode) return;

    if (currentState == GameState::PLAYING && is_flag_mode) {
        game_board_.flagCell(cell_x, cell_y);
        return;
    }

    if (currentState == GameState::PLAYING) {
        const Cell& cell = game_board_.getCell(cell_x, cell_y);

        if (cell.isRevealed() && cell.content_->getValue() > 0) {
            game_board_.chordCell(cell_x, cell_y);
            return;
        }
        game_board_.revealCell(cell_x, cell_y);
        return;
    }

    try {
        game_board_.populate(cell_x, cell_y);

    } catch (const std::exception& e) {
        std::cerr << "Game Setup Error: " << e.what() << "\n";
        game_board_.setGameState(GameState::LOST);
    }
}
