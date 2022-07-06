#ifndef CONSOLE_TETRIS_FONTHELPER_H
#define CONSOLE_TETRIS_FONTHELPER_H


#include "../tetris.h"

class FontHelper {
public:
    static void drawNumber(Point *point, float number, bool clear = false);

    static void drawDigit(Point *point, short digit, bool clear = false);

    static void drawCharacter(Point *point, char character, bool clear = false);
};


#endif //CONSOLE_TETRIS_FONTHELPER_H
