#ifndef CONSOLE_TETRIS_TETROMINOS_H
#define CONSOLE_TETRIS_TETROMINOS_H


#include "../Tetris.h"
#include "../helper/TetrisHelper.h"

class TetrominoS: public Tetris {
public:
    void rotate() override;

    explicit TetrominoS(int startX = 0, int startY = START_Y);
};


#endif //CONSOLE_TETRIS_TETROMINOS_H
