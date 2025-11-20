#include "Game.h"
#include <iostream>

void Game::displayBoard(sf::RenderWindow& window) {

}

void Game::initializeGraphics() {
    if (!font_.loadFromFile("../gui/Nunito-Black.ttf")) {
        std::cerr << "Error: Failed to load font!\n";
        exit(1);
    }
        //возможно потом добавить там для других объектов картинки хз
}

void Game::processClick(int cell_x, int cell_y, bool is_flag_mode) {

}

void Game::processMouseClick(sf::Mouse::Button, int cell_x, int cell_y) {

}

void Game::run() {

}

void Game::setGameState(GameState state) {
    current_state_ = state;
}

Game::Game(int w, int h, int bombs) : game_board_(Board(w, h, bombs)), current_state_(GameState::READY) {
    initializeGraphics();
}