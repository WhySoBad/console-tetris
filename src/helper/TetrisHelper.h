#ifndef CONSOLE_TETRIS_TETRISHELPER_H
#define CONSOLE_TETRIS_TETRISHELPER_H

#include "vector"
#include "../Tetris.h"
#include "../Game.h"

#define TOP_LEFT_CORNER         "┏"
#define TOP_RIGHT_CORNER        "┓"
#define BOTTOM_LEFT_CORNER      "┗"
#define BOTTOM_RIGHT_CORNER     "┛"
#define VERTICAL_BORDER         "┃"
#define HORIZONTAL_BORDER       "━"
#define TOP_CROSSING_BORDER     "┳"
#define BOTTOM_CROSSING_BORDER  "┻"
#define RIGHT_CROSSING_BORDER   "┫"
#define LEFT_CROSSING_BORDER    "┣"
#define EMPTY_SPACE             " "
#define OCCUPIED_SPACE          " "
#define PREVIEW_DOT             "•"

#define FIELD_WIDTH     10 // width of the game field
#define MENU_WIDTH      7 // width of the right sided menu
#define UPCOMING_HEIGHT 6 // height of the upcoming box
#define FIELD_HEIGHT    24 // height of the game field
#define START_Y         -2
// #define POINT_SIZE      1 // ui size [1 = small; 2 = big]
#define LETTER_SIZE     1
#define POINT_WIDTH     2 // point width [2 for smoother bricks]
#define LETTER_HEIGHT   5
#define LETTER_WIDTH    3

class TetrisHelper {
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
     * Get a point of the fixed array
     * @param x x-coordinate of the point
     * @param y y-coordinate of the point
     * @return short [0 = point not fixed; all other numbers equal the color of the point]
     */

    [[nodiscard]] static short getFixedPoint(int x, int y);

    /**
     * Set a point in the fixed array
     * @param x x-coordinate of the point
     * @param y y-coordinate of the point
     * @param value new value of the point
     */

    static void setFixedPoint(int x, int y, short value);

    /**
     * Get a row of the fixed array
     * @param y y-coordinate of the row
     * @return row of the fixed array
     */

    [[nodiscard]] static array<short, FIELD_WIDTH> getFixedLine(int y);

    /**
     * Set a row in the fixed array
     * @param y y-coordinate of the row
     * @param row new row
     */

    static void setFixedLine(int y, array<short, FIELD_WIDTH> row);

    /**
     * Get the whole fixed array
     * @return fixed array
     */

    [[nodiscard]] static array<array<short, FIELD_WIDTH>, FIELD_HEIGHT> getFixed();

    /**
     * Generate a new tetromino
     * @return a new tetromino
     */

    [[nodiscard]] static Tetris* generateTetromino();

    /**
     * Create a copy of a tetromino
     * @param tetris tetromino to copy
     * @return copy of the tetromino
     */

    [[nodiscard]] static Tetris *copyTetromino(Tetris *tetris);

    /**
     * Initialize the tetris helper
     */

    static void initialize();

private:
    static inline array<array<short, FIELD_WIDTH>, FIELD_HEIGHT> fixed{};
    static inline array<short, 7> tetrominoBlock;
    static inline short currentBlock = 0;
};


#endif //CONSOLE_TETRIS_TETRISHELPER_H
