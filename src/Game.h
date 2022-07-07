#ifndef CONSOLE_TETRIS_GAME_H
#define CONSOLE_TETRIS_GAME_H

#include "ncurses.h"
#include "string"
#include "vector"
#include "Tetris.h"

using namespace std;

enum Direction {
    vertical, horizontal
};

class Game {
public:

    /**
     * Start the game
     */

    void start();

    /**
     * Stop the game
     */

    void stop();

    /**
     * Reset the game
     */

    void reset();

    Game();

private:

    /**
     * Fix the active tetris object
     */

    void fixActive();

    /**
     * Handle a game over
     */

    static void gameOver();

    /**
     * Update method
     */

    void draw();

    /**
     * Get the current framerate of the game depending on the game progress
     * @return framerate
     */

    [[nodiscard]] int getDroppingFrames() const;


    Tetris *active;
    Tetris *upcoming;
    int level = 0;
    int score = 0;
    int cleared = 0;
    int framerate;
    bool running = false;
    bool paused = false;
};

#endif