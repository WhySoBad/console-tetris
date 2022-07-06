#ifndef CONSOLE_TETRIS_DRAWHELPER_H
#define CONSOLE_TETRIS_DRAWHELPER_H


#include "../Tetris.h"
#include "TetrisHelper.h"

class DrawHelper {
public:

    /**
     * Draw a tetris object
     * @param tetris tetris object to be drawn
     * @param character character to fill the tetris with
     * @param color color of the tetris
     */

    static void drawTetris(Tetris *tetris, const char * character, int color = -1);

    /**
     * Draw a tetris with it's default color
     * @param tetris tetris to be drawn
     */

    static void drawTetris(Tetris *tetris);

    /**
     * Draw the paused icon into the upcoming box
     */

    static void drawPaused();

    /**
     * Clear a tetris object
     * @param tetris tetris object to be cleared
     */

    static void clearTetris(Tetris *tetris);

    /**
     * Clear the field
     */

    static void clearField();

    /**
     * Draw a point
     * @param point point to be drawn at
     * @param character character to fill the point
     * @param color color of the point
     * @param ignore boolean whether to ignore if the point is out of screen
     * @param letter boolean whether the point is a letter [to use letter size]
     */

    static void drawPoint(Point *point, const char * character, int color = -1, bool ignore = false, bool letter = false);

    /**
     * Draw the field border
     */

    static void drawBorder();

    /**
     * Draw the preview line for a dropping tetris object
     * @param tetris tetris object
     * @param character character of the preview line
     */

    static void drawPreviewLine(Tetris * tetris, const char *character = PREVIEW_DOT);

    /**
     * Draw the upcoming tetris
     * @param tetris tetris object to be drawn
     */

    static void drawUpcoming(Tetris *tetris);

    /**
     * Clear the upcoming tetris space
     */

    static void clearUpcoming();

    /**
     * Draw the statistics
     * @param score current score
     * @param level current level
     * @param lines current cleared lines
     */

    static void drawStatistics(int score, int level, int lines);

    /**
     * Clear the drawn statistics
     */

    static void clearStatistics();

    /**
     * Enable or disable a color pair
     * @param color color pair id
     * @param disable boolean whether the color pair should be disabled
     */

    static void useColor(int color, bool disable = false);

    /**
     * Print characters at a given position
     * @param x x-coordinate
     * @param y y-coordinate
     * @param characters characters to print
     */

    static void printAt(int x, int y, const char * characters);

    /**
     * Get the field's padding on the x-axis
     * @return padding on the x-axis
     */

    [[nodiscard]] static int getPaddingX();

    /**
     * Get the field's padding on the y-axis
     * @return padding on the y-axis
     */

    [[nodiscard]] static int getPaddingY();



    /**
     * Initialize the draw helper
     */

    static void initialize();

private:
    static WINDOW * window;
    static inline int paddingX = 0;
    static inline int paddingY = 0;
};


#endif //CONSOLE_TETRIS_DRAWHELPER_H
