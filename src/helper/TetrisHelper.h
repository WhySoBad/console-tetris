#ifndef CONSOLE_TETRIS_CONSOLEHELPER_H
#define CONSOLE_TETRIS_CONSOLEHELPER_H


#include "vector"
#include "../tetris.h"
#include "../game.h"

class ConsoleHelper {
public:
    /**
     * Check whether a point is in screen
     * @param point point to be checked
     * @return boolean whether the point is in screen
     */

    [[nodiscard]] static bool isPointInScreen(Point *point);

    /**
     * Check whether a point is in field
     * @param point point to be checked
     * @return boolean whether the point is in field
     */

    [[nodiscard]] static bool isPointInField(Point *point);

    /**
     * Check whether a tetris can rotate
     * @param tetris tetris to be checked
     * @return boolean whether the tetris can rotate
     */

    [[nodiscard]] static bool canTetrisRotate(Tetris *tetris);

    /**
     * Check whether a tetris can move
     * @param tetris tetris to be checked
     * @return boolean whether the tetris can move
     */

    [[nodiscard]] static bool canTetrisMove(Tetris *tetris, Direction direction, int delta);

    /**
     * Check whether a tetris does intersect with other stuff
     * @param tetris tetris to be checked
     * @return boolean whether the tetris does intersect
     */

    [[nodiscard]] static bool doesTetrisIntersect(Tetris *tetris);

    /**
     * Get the highest occupied y-coordinate at a x-coordinate
     * @param x x-coordinate
     * @param higher y-coordinate below which should be checked
     * @return highest occupied y-coordinate at a x-coordinate
     */

    [[nodiscard]] static int getHighestFixed(int x, int higher = 0);

    /**
     * Initialize the console helper
     * @param fixed vector with the fixed fields
     */

    static void initialize(vector<vector<short>> &fixed);

private:
    static vector<vector<short>> fixed;
};


#endif //CONSOLE_TETRIS_CONSOLEHELPER_H
