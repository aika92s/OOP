#ifndef MINESWEEPERLAB_COLORS_H
#define MINESWEEPERLAB_COLORS_H
#pragma once
#include <SFML/Graphics.hpp>

namespace Colors {
    const sf::Color OutlineColor     = sf::Color(100, 100, 100);
    const sf::Color HiddenCellColor  = sf::Color(150, 150, 150);
    const sf::Color RevealedCellColor = sf::Color(220, 220, 220);
    const sf::Color OverlayColor     = sf::Color(255, 255, 255, 150);
    const sf::Color Background = sf::Color(240, 240, 240);
}

#endif //MINESWEEPERLAB_COLORS_H