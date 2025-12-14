#ifndef GAME_STATE_H
#define GAME_STATE_H

enum class GameState : signed char {
    READY, //the state of the game before the first move.
    PLAYING, //the main state of the game: the user interacts with the cells
    WON,
    LOST
};

#endif